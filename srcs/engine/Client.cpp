/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:18:09 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/03 15:52:40 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <iostream>

Client::Client()
{
	_fd = 0;
	memset(_host, 0, sizeof(_host));
	memset(_service, 0, sizeof(_service));
	memset(&_addr, 0, sizeof(_addr));
	_len = sizeof(_addr);
}

Client::Client(int socket)
{
	memset(_host, 0, sizeof(_host));
	memset(_service, 0, sizeof(_service));
	memset(&_addr, 0, sizeof(_addr));
	_len = sizeof(_addr);
	_fd = accept(socket, (struct sockaddr *)&_addr, &_len);
	if (_fd < 0)
		error("accept");
	if (isValid() && !setNonBlocking(_fd))
		return ;
	closeFd();
}

void	Client::getInfo()
{	
	getnameinfo((struct sockaddr *)&_addr, _len, _host,
		sizeof(_host), _service, sizeof(_service),
		NI_NUMERICHOST | NI_NUMERICSERV);
}

Client::Client(const Client &src)
{
	*this = src;
}

Client& Client::operator=(const Client &src)
{
	_fd = src._fd;
	_addr = src._addr;
	_len = src._len;
	_request = src._request;
	return *this;
}

Client::~Client()
{
}

int	Client::isValid() const
{
	return fcntl(_fd, F_GETFD) != -1 || errno != EBADF;
}

int	Client::getFd() const
{
	return _fd;
}

int	Client::closeFd()
{
	if (isValid())
		return close(_fd);
	_fd = -1;
	return EXIT_SUCCESS;
}

const char*	Client::getHost() const
{
	return (const char *) _host;
}

const char*	Client::getService() const
{
	return (const char *) _service;
}

int	Client::rcvRequest()
{
	char	buffer[BUFFER_SIZE];
	ssize_t	bytes_read;

	bytes_read = recv(_fd, buffer, BUFFER_SIZE, 0);
	if (bytes_read < 0)
	{
		error("recv");
		closeFd();
		return EXIT_FAILURE;
	}
	if (bytes_read == 0)
	{
		std::cout << RED "Empty request. Closing connection.\n" RESET;
		closeFd();
		return EXIT_FAILURE;
	}
	buffer[bytes_read] = '\0';
	std::cout << GREEN "Received request from ";
	std::cout << *this << "\n" << buffer << RESET "\n";
	_request = Request(buffer);
	return EXIT_SUCCESS;
}

int	Client::sendResponse(Config & config)
{
	std::string	response = _request.response(config);
	ssize_t		bytes_sent;

	std::cout << YELLOW "Sending response to " << *this << ":\n";
	std::cout << response << RESET "\n";
	bytes_sent = send(_fd, response.c_str(), response.size(), 0);
	if (bytes_sent < 0 || bytes_sent == 0)
	{
		error("send");
		closeFd();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

std::ostream &	operator<<(std::ostream & os, Client	&client)
{
	client.getInfo();
	os << client.getHost() << ":" << client.getService();
	return os;
}

bool	Client::keepAlive()
{
	return _request.keepAlive();
}