/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:18:19 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/13 10:57:07 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include <iostream>

Message::Message()
{
	_complete = true;
	_fail = false;
}

Message::Message(t_lines lines)
{
	_fail = false;
	_complete = true;
	if (parseHeaders(lines) || parseBody(lines))
		_fail = true;
}

Message::Message(std::string raw)
{
	t_lines	lines = split< t_lines >(raw, CRLF);
	
	*this = Message(lines);
}

Message::Message(std::string raw, bool skipFirstLine)
{
	t_lines lines = split< t_lines >(raw, CRLF);
	
	if (skipFirstLine)
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
	return *this;
}

Message::~Message()
{
}

int	Message::parseHeader(std::string &line)
{
	std::istringstream	lineStream(line);
	std::string			key, value;
	
	std::getline(lineStream, key, ':');
	std::getline(lineStream >> std::ws, value);
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