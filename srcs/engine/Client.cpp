/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:18:09 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/16 18:22:43 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Logs.hpp"
#include "AMethod.hpp"
#include "Get.hpp"
#include "Delete.hpp"
#include "Post.hpp"

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
	_virtualServers = NULL;
	memset(&_addr, 0, sizeof(_addr));
}

Client::Client(int socket, t_virtualServers *virtualServers)
{
	_virtualServers = virtualServers;
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
	_virtualServers = src._virtualServers;
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
		Logs(BLUE) << *this << " closing connection\n";
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

int	Client::handleTLSConnection()
{
	Logs(RED) << *this << " initiating unsupported TLS connection\n";
	closeFd();
	return EXIT_FAILURE;
}

int	Client::receive()
{
	char		buffer[BUFFER_SIZE];
	ssize_t		bytes_read;

	bytes_read = recv(_fd, buffer, BUFFER_SIZE, 0);
	if (bytes_read <= 0)
	{
		if (bytes_read == 0)
			Logs(BLUE) << *this << " client closed connection \n";
		else
			error("recv");
		closeFd();
		return EXIT_FAILURE;
	}
	_received.clear();
	while (bytes_read > 0)
	{
		_received.append(buffer, bytes_read);
		bytes_read = recv(_fd, buffer, BUFFER_SIZE, 0);
	}
	return EXIT_SUCCESS;
}

int	Client::rcvRequest()
{
	if (receive())
		return EXIT_FAILURE;
	if (_received[0] == '\026')
		return handleTLSConnection();
	if (!_request.complete())
	{
		_request.addNewChunks(_received.c_str()); //todo @leontinepaq changer en string
		Logs(GREEN) << *this << " " << "New Chunk\n";
	}
	else
	{
		logRequest(this, _received);
		_request = Request(_received);
	}
	return EXIT_SUCCESS;
}

static void	logResponse(Client *client, std::string & response)
{
	Logs(YELLOW) << *client << " "
		<< response.substr(0, response.find_first_of('\n', 0))
		<< "\n";
}

void	Client::setResponse()
{
	AMethod		*method;
	std::string	out;
	Config&		config = getConfig();

	if (_request.getMethod() == "POST")
		method = new Post(config, _request);
	else if (_request.getMethod() == "DELETE")
		method = new Delete(config, _request);
	else
		method = new Get(config, _request);
	_response = method->getResponse();
	delete method;
}

Config&	Client::getConfig() const
{
	std::string	server_name = _request.getHeader("Host");
	
	if (server_name == ""
	|| _virtualServers->find(server_name) == _virtualServers->end())
		server_name = "default";
	return _virtualServers->at(server_name);
}

int	Client::sendResponse()
{
	std::string	response;
	ssize_t		bytes_sent;
	
	if (!_request.complete())
		return EXIT_SUCCESS;
	setResponse();
	response = _response.getResponse(getConfig());
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
	return _request.keepAlive() && _response.keepAlive();
}
