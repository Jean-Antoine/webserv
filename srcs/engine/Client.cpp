/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:18:09 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/22 09:18:25 by jeada-si         ###   ########.fr       */
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

Client::Client():
	_fd(0),
	// _sessionId(""),
	_virtualServers(NULL),
	_timeout(false)
{
	memset(&_addr, 0, sizeof(_addr));
}

Client::Client(int socket, t_virtualServers *virtualServers):
	_len(sizeof(_addr)),
	// _sessionId(""),
	_virtualServers(virtualServers),
	_timeout(false)
{
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
	_fd = src._fd;
	_addr = src._addr;
	_len = src._len;
	_host = src._host;
	_service = src._service;
	// _sessionId = src._sessionId;
	_virtualServers = src._virtualServers;
	_received = src._received;
	_request = src._request;
	_response = src._response;
	_timeout = src._timeout;
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

// const std::string &	Client::getSessionId() const
// {
// 	return _sessionId;
// }

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

int	Client::checkRecv()
{
	char	buffer;
	ssize_t	bytes_read;

	
	bytes_read = recv(_fd, &buffer, 1, MSG_PEEK);
	if (bytes_read <= 0)
	{
		if (bytes_read == 0)
		{
			Logs(BLUE) << *this << " client closed connection \n";
			return EXIT_FAILURE;
		}
		if (bytes_read < 0)
			error("recv");
		closeFd();
		return EXIT_FAILURE;
	}	
	if (buffer == '\26')
		return handleTLSConnection();
	return EXIT_SUCCESS;
}
// en cas de timeout de la reception de requete : response code = 408 + header connection close

int	Client::rcvRequest()
{
	char		buffer[BUFFER_SIZE];
	ssize_t		bytes_read = 0;
	time_t		start = std::time(NULL);

	_timeout = false;
	if (checkRecv())
		return EXIT_FAILURE;
	_received.clear();
	while (true)
	{
		if (bytes_read > 0)
			_received.append(buffer, bytes_read);
		_timeout = std::difftime(std::time(NULL), start) >= RCV_TIMEOUT;
		if (_timeout)
		{
			Logs(RED) << *this << " request timeout\n";
			return EXIT_SUCCESS;
		}
		bytes_read = recv(_fd, buffer, BUFFER_SIZE, 0);
		if (bytes_read <= 0)
		{
			Message message(_received, true);
			if (message.complete())
				break ;
		}
	}
	logRequest(this, _received);
	_request = Request(_received);
	// if (_sessionId == "" && !_request.getSession().empty())
	// {
	// 	_sessionId = _request.getSession();
	// 	Logs(ORANGE) << "Resuming session " <<  _sessionId << "\n";
	// }
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
	if (_timeout)
	{
		_response = Response(true);
		return;
	}
	
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
		
	setResponse();
	// if (_sessionId == "" && _request.getSession() == "")
	// {
	// 	_sessionId = _response.setSession();
	// 	Logs(ORANGE) << *this << " attributing session id "
	// 		<< _sessionId << "\n";
	// }
	// else if (_request.getSession() == "")
	// {
	// 	Logs(ORANGE) << _sessionId << " expired\n";
	// 	_sessionId = _response.setSession();
	// 	Logs(ORANGE) << *this << " attributing new session id "
	// 		<< _sessionId << "\n";
	// }
	response = _response.getResponse(getConfig());
	if (!g_run)
		return EXIT_FAILURE;
	logResponse(this, response);
	bytes_sent = send(_fd, response.c_str(), response.size(), 0); // @@Jean-Antoine verifier si le comportement quand send fail te parait bon
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
