/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:37:29 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/27 09:01:37 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>

extern int g_run;

Server::Server(JsonData& config):
	_config(config),
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
	_epoll = epoll_create(1);
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
	std::cerr << RED << prefix << ": ";
	std::cerr << strerror(errno) << RESET "\n";
	return EXIT_FAILURE;
}

void	Server::getAdress()
{
	memset(&_hints, 0, sizeof(_hints));
	_hints.ai_family = AF_UNSPEC;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_flags = AI_PASSIVE;
	getaddrinfo(NULL, PORT, &_hints, &_res);
}

int	Server::addToPoll(int fd)
{
	epoll_event	event;
	event.events = EPOLLIN | EPOLLET;
	event.data.fd = fd;
	return epoll_ctl(_epoll, EPOLL_CTL_ADD, fd, &event);
}

int	Server::acceptConnection()
{
	struct sockaddr_storage	addr;
	socklen_t				len;
	int						fd;

	len = sizeof(addr);	
	fd = accept(_socket, (struct sockaddr *)&addr, &len);
	if (fd < 0)
		return error("accept");
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
	int		bytes_read;

	bytes_read = recv(fd, buffer, BUFFER_SIZE, 0);
	if (bytes_read <= 0)
	{
		ft_close(fd);
		return error("recv");
	}
	buffer[bytes_read] = '\0';
	std::cout << "Received request:\n" << buffer << std::endl;
	std::string response;
	response.append(buffer);
	send(fd, response.c_str(), response.size(), 0);
	ft_close(fd);
	return EXIT_SUCCESS;
}

int	Server::run()
{
	epoll_event	events[MAX_EVENTS];
	
	std::cout << BLUE "Server listening on port " << PORT << "...\n" RESET;
	while (g_run)
	{
		int	count = epoll_wait(_epoll, events, MAX_EVENTS, 10);
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