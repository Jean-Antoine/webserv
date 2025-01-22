/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:18:19 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/22 16:03:36 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include "Logs.hpp"
#include <iostream>

Message::Message():
	_fail(false),
	_complete(false)
{
}

Message::Message(t_lines lines):
	_fail(false),
	_complete(false)
{
	if (parseHeaders(lines) || parseBody(lines))
		_fail = true;
}

Message::Message(std::string raw):
	_fail(false),
	_complete(false)
{
	t_lines	lines = split< t_lines >(raw, CRLF);
	
	*this = Message(lines);
}

Message::Message(std::string raw, bool skipFirstLine):
	_fail(false),
	_complete(false)
{	
	t_lines lines = split< t_lines >(raw, CRLF);
	
	if (skipFirstLine && !lines.empty())
		lines.pop_front();
	*this = Message(lines);
}

Message::Message(const Message &src)
{
	*this = src;
}

Message& Message::operator=(const Message &src)
{
	if (this == &src)
		return *this;
	_headers = src._headers;
	_body = src._body;
	_fail = src._fail;
	_complete = src._complete;
	// _cookies = src._cookies;
	return *this;
}

Message::~Message()
{
}

// static t_cookie	newCookie(std::string name, std::string value)
// {
// 	t_cookie	cookie;

// 	cookie._name = name;
// 	cookie._value = value;
// 	cookie._expires = "";
// 	cookie._maxAge = 0;
// 	cookie._path = "";
// 	cookie._domain = "";
// 	return cookie;
// }

// int	Message::parseCookies(std::string &value)
// {
// 	t_lines	cookies = split< t_lines >(value, ";");
	
// 	while (!cookies.empty())
// 	{
// 		std::string	pair = cookies.front();
// 		std::size_t	sep;

// 		cookies.pop_front();
// 		trimSpaces(pair);
// 		sep = pair.find_first_of("=");
// 		if (pair.empty())
// 			continue;
// 		if (sep == std::string::npos)
// 		{
// 			Logs(RED) < "Cookie parsing failed \n";
// 			return EXIT_FAILURE;
// 		}

// 		std::string	name = pair.substr(0, sep);
// 		std::string value = pair.substr(sep + 1, std::string::npos);

// 		_cookies[name] =
// 			newCookie(name, value);
// 	}
// 	return EXIT_SUCCESS;
// }

int	Message::parseHeader(std::string &line)
{
	std::istringstream	lineStream(line);
	std::string			key, value;
	
	std::getline(lineStream, key, ':');
	std::getline(lineStream >> std::ws, value);
	// if (key == "Cookie")
	// 	return parseCookies(value);
	if (lineStream.fail())
		return EXIT_FAILURE;
	_headers[key] = value;
	return EXIT_SUCCESS;
}

int	Message::parseHeaders(t_lines &lines)
{	
	while (!lines.empty() && !lines.front().empty())
	{
		if (parseHeader(lines.front()))
		{
			Logs(RED) < "Header parsing failed\n";
			return EXIT_FAILURE;
		}
		lines.pop_front();
	}	
	if (!lines.front().empty())	
		return EXIT_FAILURE;
	if (!lines.empty())
		lines.pop_front();
	return EXIT_SUCCESS;
}

int Message::parseBody(t_lines &lines)
{
	if (_headers["Transfer-Encoding"] == "chunked")
	{
		_complete = false;
		*this += Chunk(lines);
		return EXIT_SUCCESS;
	}
	while (!lines.empty())
	{
		_body.append(lines.front() + CRLF);
		lines.pop_front();
	}
	_complete = getContentLength() <= _body.size();
	return EXIT_SUCCESS;
}

Message&	Message::operator+=(const Message &src)
{
	for (t_headers::const_iterator it = src._headers.begin();
	it != src._headers.end(); it++)
		this->_headers[it->first] = it->second;
	this->_body += src._body;
	_fail = _fail || src._fail;
	return *this;
}

Message	Message::operator+(const Message &src)
{
	Message out = *this;

	out += src;
	return out;
}

Message& Message::operator+=(const Chunk &src)
{
	this->_body += src.body();
	_complete = src.complete();
	_fail = _fail || src.fail();
	return *this;
}

Message	Message::operator+(const Chunk &src)
{
	Message	out = *this;

	out += src;
	return out;
}

int	Message::addNewChunks(const char *buffer)
{
	if (_complete)
		return EXIT_FAILURE;
		
	*this += Chunk(buffer);
	return !_fail;
}

void	Message::setHeader(const std::string & key, const std::string & value)
{
	_headers[key] = value;
}

const std::string &	Message::getHeader(const char *key) const
{
	if (isHeaderSet(key))
		return _headers.find(key)->second;
	return empty::string;
}

// t_cookie	Message::getCookie(const char *key) const
// {
// 	if (_cookies.find(key) != _cookies.end())
// 		return _cookies.find(key)->second;
// 	return newCookie("", "");
// }

// void	Message::setCookie(std::string name, std::string value)
// {
// 	_cookies[name] = newCookie(name, value);
// }

// void	Message::setCookie(t_cookie & cookie)
// {
// 	_cookies[cookie._name] = cookie;
// }

double	Message::getContentLength() const
{
	if (!isHeaderSet("Content-Length"))
		return 0;
	
	char*	end;
	double	out = std::strtod(getHeader("Content-Length").c_str(), &end);
	if (*end != '\0')
		return 0;
	return out;
}

bool	Message::isHeaderSet(const char *key) const
{
	return _headers.find(key) != _headers.end();
}

void	Message::setBody(const char *str)
{
	_body = str;
}

void	Message::setBody(std::string str)
{
	_body = str;
}

const std::string &	Message::getBody() const
{
	return _body;
}

bool	Message::fail() const
{
	return _fail;
}

bool	Message::complete() const
{
	return _complete;
}

bool	Message::keepAlive() const
{
	if (!isHeaderSet("Connection"))
		return true;
	if (getHeader("Connection") == "close")
		return false;
	return true;
}

// std::string	Message::getSession() const
// {
// 	return getCookie("session_id")._value;
// }
// 
// static std::string randStr(int size) {
// 	std::string	chars =	"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
// 	std::string randomString;
// 
// 	for (int i = 0; i < size; ++i)
// 		randomString += chars[rand() % (chars.size() - 1)];
// 	return randomString;
// }
// 
// std::string Message::setSession()
// {
// 	std::string	id = randStr(10);
// 	t_cookie	session = newCookie("session_id", id);
// 
// 	session._maxAge = SESSION_MAX_AGE;
// 	session._path = "/";
// 	setCookie(session);
// 	return id;
// }
