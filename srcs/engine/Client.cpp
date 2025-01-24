/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:18:09 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/24 15:39:28 by jeada-si         ###   ########.fr       */
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

static void	log(Client *client, std::string & response, bool request)
{
	if (response.empty())
		return ;
	
	const char	*color;
	t_lines lines = split <t_lines>(response, CRLF);
	if (request)
		color = std::string(GREEN).c_str();
	else
		color = std::string(BLUE).c_str();
	if (request)
		Logs(color) << "[<] " << *client 
			<< " receiving request " << lines[0] <<"\n";
	else
		Logs(color) << "[>] " << *client 
			<< " sending response " << lines[0] << "\n";
	for (t_lines::const_iterator it = lines.begin();
	it != lines.end() && !it->empty(); it++)
		Logs(color) < *it < "\n";
}

int	Client::handleTLSConnection()
{
	Logs(RED) << *this << " initiating unsupported TLS connection\n";
	return EXIT_FAILURE;
}

static time_t	elapsedSince(time_t date)
{
	return std::difftime(std::time(NULL), date);
}

int	Client::checkRecv(const char *buffer, ssize_t bytes)
{
	if (bytes == 0)
		Logs(CYAN) << "[-] " << *this
			<< " client closed connection \n";
	if (bytes < 0)
		error("recv");
	if (bytes <= 0)
		return EXIT_FAILURE;
	if (buffer[0] == '\26')
		return handleTLSConnection();
	return EXIT_SUCCESS;
}

int	Client::rcvRequest()
{
	char	buffer[BUFFER_SIZE];
	ssize_t	bytes;

	if (_received.empty())
		_start = std::time(NULL);
	if (elapsedSince(_start) > RCV_TIMEOUT)
		_timeout = true;
	bytes = recv(_fd, buffer, BUFFER_SIZE, 0);
	if (checkRecv(buffer, bytes))
		return EXIT_FAILURE;
	_request = _received.append(buffer, bytes);
	if (_request.complete())
	{
		setResponse();
		log(this, _received, true);
	}
	return EXIT_SUCCESS;
}

bool	Client::ready() const
{
	return _request.complete() || _request.fail() || timeout();
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
	
	if (_timeout)
	{
		_response = Response(true);
		return ;
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

void	Client::manageSession()
{
	if (_request.getSession().empty())
	{
		_sessionId = _response.setSession();
		Logs(ORANGE) << "[i] " << *this
			<< " session_id set to [" << _sessionId << "]\n";
	}
	else if (_sessionId.empty())
	{
		_sessionId = _request.getSession();
		Logs(ORANGE) << "[i] " << *this
			<< " resuming session [" << _sessionId << "]\n";
	}
	incrSessionCount();
	Logs(ORANGE) << "[i] [" << _sessionId
		<< "] number of successful requests: "
		<< getSessionCount() << "\n";
	_response.setCookie("request_count", to_string(getSessionCount()), false);
}

int	Client::sendResponse()
{
	std::string	response;
	ssize_t		bytes;

	if (!g_run)
		return EXIT_FAILURE;
	response = _response.getResponse(getConfig());
	log(this, response, false);
	bytes = send(_fd, response.c_str(), response.size(), MSG_NOSIGNAL);
	if (bytes <= 0)
	{
		error("send");
		return EXIT_FAILURE;
	}
	_request = Request();
	_received.clear();
	return EXIT_SUCCESS;
}

bool	Client::keepAlive()
{
	return _request.keepAlive() && _response.keepAlive();
}


void Client::incrSessionCount()
{
	if (_sessionId.empty())
		return ;
	if (_sessions.find(_sessionId) == _sessions.end())
		_sessions[_sessionId] = 0;
	else
		_sessions[_sessionId] += 1;
}

int Client::getSessionCount()
{
	if (_sessions.find(_sessionId) == _sessions.end())
		return 0;
	return _sessions[_sessionId];
}