/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:37:29 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/27 14:17:32 by lpaquatt         ###   ########.fr       */
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
	_socket = socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol);
	if (_socket < 0)
		return error("socket");
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		return error("setsockopt");
	if (fcntl(_socket, F_SETFL, O_NONBLOCK) < 0)
		return error("fcntl");
	if (bind(_socket, _res->ai_addr, _res->ai_addrlen) < 0)
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
	freeaddrinfo(_res);
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
	memset(&_hints, 0, sizeof(_hints));
	_hints.ai_family = AF_UNSPEC;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_flags = AI_PASSIVE;
	getaddrinfo(
		_config.host(),
		to_string(_config.port()).data(),
		&_hints,
		&_res
		);
}

int	Server::addToPoll(int fd)
{
	epoll_event	event;
	event.events = EPOLLIN;
	event.data.fd = fd;
	return epoll_ctl(_epoll, EPOLL_CTL_ADD, fd, &event);
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
	if (getnameinfo((struct sockaddr *)&addr, len, host,
		sizeof(host), service, sizeof(service),
		NI_NUMERICHOST | NI_NUMERICSERV) == 0)
		std::cout << PINK "New connection from " << host << ":" << service << RESET "\n";
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
	{
		ft_close(fd);
		error("fcntl");
	}
	return addToPoll(fd);
}

int Server::handleClient(int fd)
{
	char	buffer[BUFFER_SIZE];
	ssize_t	bytes_read;
	ssize_t	bytes_sent;

	bytes_read = recv(fd, buffer, BUFFER_SIZE, 0);
	if (bytes_read < 0)
	{
		ft_close(fd);
		return error("recv");
	}
	buffer[bytes_read] = '\0';
	std::cout << GREEN "Received request:\n" << buffer << RESET "\n";
	std::string response;
	response.append("HTTP/1.1 200 OK\r\n\r\n");
	response.append(buffer);
	std::cout << YELLOW "Responding:\n";
	std::cout << response << std::endl;
	bytes_sent = send(fd, response.c_str(), response.size(), 0);
	ft_close(fd);
	epoll_ctl(_epoll, EPOLL_CTL_DEL, fd, NULL);
	if (bytes_sent < 0)
		return error ("send");
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
		int	count = epoll_wait(_epoll, events, MAX_EVENTS, 100);
		if (count < 0)
			return error("epoll_wait");
		for (int i = 0; i < count ; i++)
		{
			int	fd = events[i].data.fd;
			if (fd == _socket)
				acceptConnection();
			else
				handleClient(fd);
		}
	}
	return EXIT_SUCCESS;
}