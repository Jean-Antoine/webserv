/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:37:29 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/24 15:26:36 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Logs.hpp"
#include "Client.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netdb.h>

extern int g_run;

static void	ft_close(int fd)
{
	if (fd > 0)
		close(fd);
}

static int	getAdress(struct addrinfo **addr, const char *host, int port)
{
	struct addrinfo	hints;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(host, to_string(port).data(), &hints, addr))
		return error("getaddrinfo");
	return EXIT_SUCCESS;
}

static int	getSocket(t_socket* sock, struct addrinfo *addr)
{
	int				opt = 1;
	
	*sock = socket(
		addr->ai_family,
		addr->ai_socktype,
		addr->ai_protocol);
	if (*sock < 0)
		return error("socket");
	if (setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		return error("setsockopt");
	if (setNonBlocking(*sock))
		return EXIT_FAILURE;
	if (bind(*sock, addr->ai_addr, addr->ai_addrlen) < 0)
		return error("bind");
	if (listen(*sock, BACKLOG) < 0)
		return error("listen");
	return EXIT_SUCCESS;
}

Server::Server()
{
}

t_socket	Server::addListener(t_host host, t_port port)
{
	t_hostPort			hostPort(host, port);
	t_socket			socket = -1;
	struct addrinfo		*addr = NULL;

	if (_sockets.find(hostPort) != _sockets.end())
		return _sockets[hostPort];
	if (getAdress(&addr, host.c_str(), port)
		|| getSocket(&socket, addr)
		|| addToPoll(socket))
	{
		ft_close(socket);
		Logs(RED) << "Failed to setup server "
				<< host << ":" << port << "\n";
		socket = -1;
	}
	else
	{
		Logs(CYAN) << "Listening to " 
			<< host << ":" << port << "\n";		
	}
	if (addr)
		freeaddrinfo(addr);
	_sockets[hostPort] = socket;
	return socket;
}

void	Server::addVirtualServer(t_socket socket, Config &config)
{
	if (socket == -1)
		return ;

	t_virtualServers	&virtualServers = _servers[socket];
	const t_strArray	&server_names = config.getServerNames();

	if (!virtualServers.empty() && server_names.empty())
	{
		Logs(RED) << "Missing server name for existing <host>:<port>\n";
		return ;
	}
	if (virtualServers.empty())
		virtualServers["default"] = config;
	for (t_strArray::const_iterator it = server_names.begin();
	it != server_names.end(); it++)
	{
		if (virtualServers.find(*it) == virtualServers.end())
			virtualServers[*it] = config;
		else
			Logs(RED) << "Conflicting server name " << *it << "\n";
	}
	return ;
}

Server::Server(const JsonData & data)
{
	_epoll = epoll_create(BACKLOG);
	if (_epoll < 0)
	{
		error("epoll_create");
		return ;
	}
	for (int i = 0; i < data.size(); i++)
	{
		Config			config(data[i]);
		t_socket		socket;

		if (config.check())
		{
			Logs(RED) << "Config no." << i
				<< " wrong or incomplete" << "\n";
			continue ;
		}
		socket = addListener(config.host(), config.port());
		addVirtualServer(socket, config);
	}
	g_run = false;
	for (t_servers::const_iterator it = _servers.begin();
	it != _servers.end() && !g_run; it++)
	{
		if (!it->second.empty())
			g_run = true;
	}
}

Server::~Server()
{
	Logs(RED) << "Shutting down server.\n"; 
	for (t_clients::iterator it = _clients.begin();
	it != _clients.end() ;it++)
		it->second.closeFd();
	for (t_servers::iterator it = _servers.begin();
	it != _servers.end() ;it++)
		ft_close(it->first);
	ft_close(_epoll);
}

int	Server::addToPoll(t_socket fd)
{
	epoll_event	event;
	event.events = EPOLLIN | EPOLLOUT;
	event.data.fd = fd;
	if (epoll_ctl(_epoll, EPOLL_CTL_ADD, fd, &event))
		return error("epoll_ctl");
	return EXIT_SUCCESS;
}

int	Server::updatePollFlag(t_socket fd, int flag)
{
	(void) flag;
	epoll_event event;
	// event.events = flag | EPOLLONESHOT;
	event.events = EPOLLIN | EPOLLOUT | EPOLLONESHOT;
	event.data.fd = fd;
	if (epoll_ctl(_epoll, EPOLL_CTL_MOD, fd, &event))
		return error("epoll_ctl");
	return EXIT_SUCCESS;
}

void	Server::rmClient(t_socket fd)
{
	epoll_ctl(_epoll, EPOLL_CTL_DEL, fd, NULL);
	_clients[fd].closeFd();
	_clients.erase(fd);
}

int	Server::acceptConnection(t_socket fd)
{
	Client	newClient(fd, &_servers[fd]);

	if (!newClient.isValid())
		return EXIT_FAILURE;
	if(addToPoll(newClient.getFd()))
	{
		newClient.closeFd();
		return EXIT_FAILURE;
	}
	_clients[newClient.getFd()] = newClient;
	Logs(CYAN) << "[+] " << newClient
		<< " attributed to socket "	<< newClient.getFd() << "\n";
	return EXIT_SUCCESS;
}

void Server::rcvRequest(t_socket fd)
{
	Client&	client = _clients[fd];
	
	if (client.rcvRequest())
		rmClient(fd);
}

void	Server::sendResponse(t_socket fd)
{
	Client&	client = _clients[fd];
	
	if (!client.ready())
		return ;
	if (client.sendResponse())
	{
		rmClient(fd);
		return ;
	}
	if (!client.keepAlive())
		rmClient(fd);
}

int	Server::run()
{
	if (!g_run)
		return EXIT_FAILURE;
	epoll_event	events[MAX_EVENTS];

	Logs(CYAN) << "Server listening...\n";
	while (g_run)
	{
		int	count = epoll_wait(_epoll, events, MAX_EVENTS, -1);
		if (!g_run)
			return EXIT_SUCCESS;
		if (count < 0)
			return error("epoll_wait");
		for (int i = 0; i < count && g_run; i++)
		{
			int	fd = events[i].data.fd;
			if (_servers.find(fd) != _servers.end())
				acceptConnection(fd);
			else if (events[i].events & EPOLLIN)
				rcvRequest(fd);
			else if (events[i].events & EPOLLOUT)
				sendResponse(fd);
		}
	}
	return EXIT_SUCCESS;
}
