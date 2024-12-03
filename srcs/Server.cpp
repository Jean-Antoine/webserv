/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:37:29 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/02 13:30:15 by lpaquatt         ###   ########.fr       */
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
	_socket = socket(
		_addr->ai_family,
		_addr->ai_socktype,
		_addr->ai_protocol);
	if (_socket < 0)
		return error("socket");
	if (setsockopt(_socket,	SOL_SOCKET,	SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		return error("setsockopt");
	if (setNonBlocking(_socket))
		return EXIT_FAILURE;
	if (bind(_socket, _addr->ai_addr, _addr->ai_addrlen) < 0)
		return error("bind");
	if (listen(_socket, BACKLOG) < 0)
		return error("listen");
	_epoll = epoll_create(BACKLOG);
	if (_epoll < 0)
		return error("epoll_create");
	if (addToPoll(_socket) < 0)
		return EXIT_FAILURE;
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
	for (t_clients::iterator it = _clients.begin();
	it != _clients.end() ;it++)
		it->second.closeFd();
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

int	Server::addToPoll(int fd)
{
	epoll_event	event;
	event.events = EPOLLIN | EPOLLET;
	event.data.fd = fd;
	if (epoll_ctl(_epoll, EPOLL_CTL_ADD, fd, &event))
		return error("epoll_ctl");
	return EXIT_SUCCESS;
}

int	Server::updatePollFlag(int fd, int flag)
{
	epoll_event event;
	event.events = flag | EPOLLET | EPOLLONESHOT;
	event.data.fd = fd;
	if (epoll_ctl(_epoll, EPOLL_CTL_MOD, fd, &event))
		return error("epoll_ctl");
	return EXIT_SUCCESS;
}

int	Server::acceptConnection()
{
	Client	newClient(_socket);

	if (!newClient.isValid())
		return EXIT_FAILURE;
	if(addToPoll(newClient.getFd()))
	{
		newClient.closeFd();
		return EXIT_FAILURE;
	}
	_clients[newClient.getFd()] = newClient;
	std::cout << PINK "New connection from ";
	std::cout << newClient << "\n" RESET;
	return EXIT_SUCCESS;
}

int Server::rcvRequest(int fd)
{
	if (_clients[fd].rcvRequest())
	{
		_clients.erase(fd);
		return EXIT_FAILURE;
	}
	if (updatePollFlag(fd, EPOLLOUT))
	{
		_clients[fd].closeFd();
		_clients.erase(fd);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int	Server::sendResponse(int fd)
{
	if (_clients[fd].sendResponse())
	{
		_clients.erase(fd);
		return EXIT_FAILURE;
	}
	if (updatePollFlag(fd, EPOLLIN))
	{
		_clients[fd].closeFd();
		_clients.erase(fd);
		return EXIT_FAILURE;
	}
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
