/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:37:29 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/29 09:07:20 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>

extern int g_run;

Server::Server(Config& Config):
	_config(Config),
	_socket(-1),
	_epoll(-1)
{
	(void) _config;
}

int	Server::setup()
{
	int	opt = 1;
	getAdress();
	_socket = socket(_addr->ai_family, _addr->ai_socktype, _addr->ai_protocol);
	if (_socket < 0)
		return error("socket");
	if (setsockopt(_socket,	SOL_SOCKET,	SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		return error("setsockopt");
	if (setNonBlocking(_socket))
		return error("fcntl");
	if (bind(_socket, _addr->ai_addr, _addr->ai_addrlen) < 0)
		return error("bind");
	if (listen(_socket, BACKLOG) < 0)
		return error("listen");
	_epoll = epoll_create(10);
	if (_epoll < 0)
		return error("epoll_create");
	if (addToPoll(_socket) < 0)
		return error("epoll_ctl");
	return EXIT_SUCCESS;
}

static void	ft_close(int fd)
{
	if (fd > 0)
		close(fd);
}

Server::~Server()
{
	std::cout << BLUE "Closing server.\n" RESET;
	ft_close(_socket);
	ft_close(_epoll);
	freeaddrinfo(_addr);
}

int	Server::error(const char *prefix)
{
	std::string out;
	
	out.append(RED);
	out.append(prefix);
	out.append(": ");
	out.append(strerror(errno));
	out.append(RESET "\n");
	std::cerr << out;
	return EXIT_FAILURE;
}

void	Server::getAdress()
{
	struct addrinfo	hints;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo(
		_config.host(),
		to_string(_config.port()).data(),
		&hints,
		&_addr
		);
}

int Server::setNonBlocking(int fd)
{
	int	flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0)
		return error("fcntl");
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
		return error("fcntl");
	return EXIT_SUCCESS;
}

int	Server::addToPoll(int fd)
{
	epoll_event	event;
	event.events = EPOLLIN | EPOLLET | EPOLLOUT;
	event.data.fd = fd;
	return epoll_ctl(_epoll, EPOLL_CTL_ADD, fd, &event);
}

// int	Server::updatePollFlag(int fd)
// {
// 	epoll_event event;
// 	event.events = EPOLLOUT;
// 	event.data.fd = fd;
// 	return epoll_ctl(_epoll, EPOLL_CTL_MOD, fd, &event);
// }

static void	printConnection(
	struct sockaddr_storage & addr,
	socklen_t & len,
	char *host,
	char *service)
{
	if (getnameinfo((struct sockaddr *)&addr, len, host,
		sizeof(host), service, sizeof(service),
		NI_NUMERICHOST | NI_NUMERICSERV))
		return ;
	std::cout << PINK "New connection from ";
	std::cout << host << ":" << service << RESET "\n";
}

int	Server::acceptConnection()
{
	struct sockaddr_storage	addr;
	socklen_t				len;
	int						fd;
	char					host[NI_MAXHOST];
	char					service[NI_MAXSERV];

	len = sizeof(addr);	
	fd = accept(_socket, (struct sockaddr *)&addr, &len);
	if (fd < 0)
		return error("accept");
	printConnection(addr, len, host, service);
	if (!setNonBlocking(fd))
		return addToPoll(fd);
	ft_close(fd);
	return error("fcntl");
}

int Server::rcvRequest(int fd)
{
	char	buffer[BUFFER_SIZE];
	ssize_t	bytes_read;

	bytes_read = recv(fd, buffer, BUFFER_SIZE, 0);
	if (bytes_read < 0)
		return error("recv");
	buffer[bytes_read] = '\0';
	_clients[fd] = Request(buffer);
	return EXIT_SUCCESS;
}

int	Server::sendResponse(int fd)
{
	Request &	Req = _clients[fd];
	std::string	response = Req.response();
	ssize_t		bytes_sent;
	char		buffer[BUFFER_SIZE];

	bytes_sent = send(fd, buffer, BUFFER_SIZE, 0);
	if (bytes_sent < 0)
		return error("send");
	return EXIT_SUCCESS;
	
}

int	Server::run()
{
	epoll_event	events[MAX_EVENTS];
	
	std::cout << BLUE "Server listening on ";
	std::cout << _config.host() << ":";
	std::cout << _config.port() << "...\n" RESET;
	while (g_run)
	{
		int	count = epoll_wait(_epoll, events, MAX_EVENTS, -1);
		if (count < 0)
			return error("epoll_wait");
		for (int i = 0; i < count ; i++)
		{
			int	fd = events[i].data.fd;
			if (fd == _socket)
				acceptConnection();
			else if (events[i].events & EPOLLIN)
				rcvRequest(fd);
			else if (events[i].events & EPOLLOUT)
				sendResponse(fd);
		}
	}
	return EXIT_SUCCESS;
}