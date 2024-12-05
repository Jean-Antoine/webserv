/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:37:29 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/05 15:09:51 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
		struct addrinfo	*addr = NULL;
		t_socket		socket = 0;
		
		if (getAdress(&addr, config.host(), config.port())
			|| getSocket(&socket, addr)
			|| addToPoll(socket))
		{
			ft_close(socket);
			std::cout << RED "Failed to setup server "
				<< config.host() << ":" << config.port()
				<< RESET "\n";
		}
		else
			_server[socket] = config;
		if (addr)
			freeaddrinfo(addr);
	}
}

Server::~Server()
{
	std::cout << BLUE "Shutting down server"
		<< ".\n" RESET;
	for (t_clients::iterator it = _client.begin();
	it != _client.end() ;it++)
		it->second.closeFd();
	for (t_servers::iterator it = _server.begin();
	it != _server.end() ;it++)
		ft_close(it->first);
	ft_close(_epoll);
}

int	Server::addToPoll(t_socket fd)
{
	epoll_event	event;
	event.events = EPOLLIN | EPOLLET;
	event.data.fd = fd;
	if (epoll_ctl(_epoll, EPOLL_CTL_ADD, fd, &event))
		return error("epoll_ctl");
	return EXIT_SUCCESS;
}

int	Server::updatePollFlag(t_socket fd, int flag)
{
	epoll_event event;
	event.events = flag | EPOLLET | EPOLLONESHOT;
	event.data.fd = fd;
	if (epoll_ctl(_epoll, EPOLL_CTL_MOD, fd, &event))
		return error("epoll_ctl");
	return EXIT_SUCCESS;
}

void	Server::rmClient(t_socket fd)
{
	_client[fd].closeFd();
	_client.erase(fd);
}

int	Server::acceptConnection(t_socket fd)
{
	Client	newClient(fd, &_server[fd]);

	if (!newClient.isValid())
		return EXIT_FAILURE;
	if(addToPoll(newClient.getFd()))
	{
		newClient.closeFd();
		return EXIT_FAILURE;
	}
	_client[newClient.getFd()] = newClient;
	std::cout << PINK "New connection from ";
	std::cout << newClient << "\n\n" RESET;
	return EXIT_SUCCESS;
}

int Server::rcvRequest(t_socket fd)
{
	if (_client[fd].rcvRequest())
	{
		rmClient(fd);
		return EXIT_FAILURE;
	}
	if (updatePollFlag(fd, EPOLLOUT))
	{
		rmClient(fd);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int	Server::sendResponse(t_socket fd)
{
	if (_client[fd].sendResponse())
	{
		rmClient(fd);
		return EXIT_FAILURE;
	}
	if (!_client[fd].keepAlive())
	{
		rmClient(fd);
		return EXIT_SUCCESS;
	}
	if (updatePollFlag(fd, EPOLLIN))
	{
		rmClient(fd);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int	Server::run()
{
	epoll_event	events[MAX_EVENTS];

	while (g_run)
	{
		int	count = epoll_wait(_epoll, events, MAX_EVENTS, -1);
		if (!g_run)
			return EXIT_SUCCESS;
		if (count < 0)
			return error("epoll_wait");
		for (int i = 0; i < count ; i++)
		{
			int	fd = events[i].data.fd;
			if (_server.find(fd) != _server.end())
				acceptConnection(fd);
			else if (events[i].events & EPOLLIN)
				rcvRequest(fd);
			else if (events[i].events & EPOLLOUT)
				sendResponse(fd);
		}
	}
	return EXIT_SUCCESS;
}