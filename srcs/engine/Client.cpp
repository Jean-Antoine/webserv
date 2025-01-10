/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:18:09 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/10 15:30:57 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

int	setNonBlocking(int fd)
{
	int	flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0 || fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
		return error("fcntl");
	return EXIT_SUCCESS;
}

Client::Client()
{
	_fd = 0;
	_config = NULL;
	memset(&_addr, 0, sizeof(_addr));
}

Client::Client(int socket, Config *config)
{
	_config = config;
	_len = sizeof(_addr);
	_fd = accept(socket, (struct sockaddr *)&_addr, &_len);
	if (_fd < 0)
		error("accept");
	getInfo();
	if (isValid() && !setNonBlocking(_fd) )
		return ;
	closeFd();
}

void	Client::getInfo()
{
	char	host[NI_MAXHOST];
	char	service[NI_MAXSERV];

	memset(&host, 0, sizeof(host));
	memset(&service, 0, sizeof(service));
	getnameinfo((struct sockaddr *)&_addr, _len, host,
		sizeof(host), service, sizeof(service),
		NI_NUMERICHOST | NI_NUMERICSERV);
	_service = std::string(service);
	_host = std::string(host);
}

Client::Client(const Client &src)
{
	*this = src;
}

Client& Client::operator=(const Client &src)
{
	_config = src._config;
	_fd = src._fd;
	_addr = src._addr;
	_len = src._len;
	_request = src._request;
	_host = src._host;
	_service = src._service;
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
	{
		Logs(RED) << "Closing connection with client "
			<< *this << "\n";
		return close(_fd);		
	}
	_fd = -1;
	return EXIT_SUCCESS;
}

const std::string &	Client::getHost() const
{
	return _host;
}

const std::string &	Client::getService() const
{
	return _service;
}

static void	logRequest(Client * client, std::string & request)
{
	Logs(GREEN) << *client << " "
		<< request.substr(0, request.find_first_of("\n")) << "\n";
}

// todo: @leon sur les chunk request ..? mettre un timeout / taille max / log si connection fermee avant d'avoir recu toute la chunk
int	Client::rcvRequest()
{
	char		buffer[BUFFER_SIZE];
	ssize_t		bytes_read;
	std::string	rcved;

	bytes_read = recv(_fd, buffer, BUFFER_SIZE, 0);
	if (bytes_read <= 0)
	{
		if (bytes_read == 0)
			Logs(RED) << *this << " closed connection \n";
		else
			error("recv");
		closeFd();
		return EXIT_FAILURE;
	}
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		rcved.append(buffer);
		bytes_read = recv(_fd, buffer, BUFFER_SIZE, 0);
	}
	if (!_request.complete())
	{
		_request.addNewChunks(rcved.c_str());
		Logs(GREEN) << *this << " " << "New Chunk\n";
	}
	else
	{
		logRequest(this, rcved);
		_request = Request(rcved.c_str());		
	}
	return EXIT_SUCCESS;
}

static void	logResponse(Client *client, std::string & response)
{
	Logs(YELLOW) << *client << " "
		<< response.substr(0, response.find_first_of('\n', 0))
		<< "\n";
}

int	Client::sendResponse()
{	
	if (!_request.complete())
		return EXIT_SUCCESS;

	std::string	response = _request.response(_config);
	ssize_t		bytes_sent;
	if (!g_run)
		return EXIT_FAILURE;
	logResponse(this, response);
	bytes_sent = send(_fd, response.c_str(), response.size(), 0);
	if (bytes_sent < 0 || bytes_sent == 0)
	{
		error("send");
		closeFd();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

bool	Client::keepAlive()
{
	return _request.keepAlive();
}

const Logs&	operator<<(const Logs& logs, Client & clt)
{
	logs.printDate(false);
	logs << clt.getHost() << ":" << clt.getService();
	return logs;
}
