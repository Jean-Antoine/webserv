/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:18:09 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/23 18:11:51 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Logs.hpp"
#include "AMethod.hpp"
#include "Get.hpp"
#include "Delete.hpp"
#include "Post.hpp"

t_sessions Client::_sessions;

int	setNonBlocking(int fd)
{
	int	flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0 || fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
		return error("fcntl");
	return EXIT_SUCCESS;
}

Client::Client():
	_fd(0),
	_virtualServers(NULL),
	_received(""),
	_timeout(false),
	_sessionId("")
{
	memset(&_addr, 0, sizeof(_addr));
}

Client::Client(int socket, t_virtualServers *virtualServers):
	_len(sizeof(_addr)),
	_virtualServers(virtualServers),
	_received(""),
	_timeout(false),
	_sessionId("")
{
	_fd = accept(socket, (struct sockaddr *)&_addr, &_len);
	if (_fd < 0)
		error("accept");
	getInfo();
	if (isValid() && !setNonBlocking(_fd))
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
	_sessionId = src._sessionId;
	_virtualServers = src._virtualServers;
	_received = src._received;
	_request = src._request;
	_response = src._response;
	_start = src._start;
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
		Logs(CYAN) << "[-] " << *this << " closing connection\n";
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

static void	log(Client *client, std::string & response, bool request)
{
	if (request)
		Logs(GREEN) << "[<] " << *client << " receiving request\n";
	else
		Logs(GREEN) << "[>] " << *client << " sending response\n";
	t_lines lines = split <t_lines>(response, CRLF);
	for (t_lines::const_iterator it = lines.begin();
	it != lines.end() && !it->empty(); it++)
		Logs(RESET) < *it < "\n";
}

int	Client::handleTLSConnection()
{
	Logs(RED) << *this << " initiating unsupported TLS connection\n";
	return EXIT_FAILURE;
}

int	Client::rcvRequest()
{
	char	buffer[BUFFER_SIZE];
	ssize_t	bytes;

	if (_received.empty())
		_start = std::time(NULL);
	if (std::difftime(std::time(NULL), _start) > RCV_TIMEOUT)
		_timeout = true;
	bytes = recv(_fd, buffer, BUFFER_SIZE, 0);
	if (bytes && buffer[0] == '\26')
		return handleTLSConnection();
	if (bytes <= 0)
	{
		if (bytes == 0)
			Logs(CYAN) << "[-] " << *this << " client closed connection \n";
		else if (bytes < 0)
			error("recv");
		return EXIT_FAILURE;
	}
	_received.append(buffer, bytes);
	Logs(GREEN) << "RECEIVING\n";
	Logs(GREEN) < _received < "\n";
	return EXIT_SUCCESS;
}

bool	Client::complete() const
{
	Message	message(_received, true);
	return message.complete() || message.fail();
}

bool	Client::timeout() const
{
	if (_timeout)
		Logs(RED) << "[x] " << *this << " request timeout\n";
	return _timeout;
}

void	Client::setResponse()
{
	AMethod		*method;
	std::string	out;
	Config&		config = getConfig();

	_request = Request(_received);
	if (getConfig().isSessionEnabled()
		&&_sessionId == "" && !_request.getSession().empty())
	{
		_sessionId = _request.getSession();
		Logs(ORANGE) << "Resuming session " <<  _sessionId << "\n";
	}
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
	
	server_name = server_name.substr(0, server_name.find_first_of(':'));
	if (server_name == ""
	|| _virtualServers->find(server_name) == _virtualServers->end())
		server_name = "default";
	return _virtualServers->at(server_name);
}

int	Client::sendResponse()
{
	std::string	response;
	ssize_t		bytes;

	if (_timeout)
		_response = Response(true);
	else
		setResponse();
	log(this, _received, true);
	_received.clear();
	if (getConfig().isSessionEnabled())
	{
		if (_sessionId.empty())
		{
			_sessionId = _response.setSession(getConfig().getSessionTimeout());
			Logs(ORANGE) << "Setting new session id: " << _sessionId << "\n";
		}
		incrementSessionReqCnt(_sessionId);
		_response.setHeader("Requests_Count", to_string(getSessionReqCnt(_sessionId)));
		Logs(ORANGE) << "Number of requests received from session_id " << _sessionId << ": " << getSessionReqCnt(_sessionId) << "\n";
	}
	response = _response.getResponse(getConfig());
	if (!g_run)
		return EXIT_FAILURE;
	log(this, response, false);
	bytes = send(_fd, response.c_str(), response.size(), 0); // @@Jean-Antoine verifier si le comportement quand send fail te parait bon
	if (bytes <= 0)
	{
		error("send");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

bool	Client::keepAlive()
{
	return _request.keepAlive() && _response.keepAlive();
}


void Client::incrementSessionReqCnt(const std::string & id)
{
	_sessions[id] = _sessions[id] + 1;
}

int Client::getSessionReqCnt(const std::string & id)
{
	return _sessions[id];
}