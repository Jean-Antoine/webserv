/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:18:19 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/10 10:44:48 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include <iostream>

Message::Message()
{
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

Message::Message(const Message &src)
{
	*this = src;
}

Message& Message::operator=(const Message &src)
{
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
			return EXIT_FAILURE;
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
	_complete = src.last();
	_fail = _fail || src.fail();
}

int	Message::addNewChunks(const char *buffer)
{
	if (_complete)
		return EXIT_FAILURE;
		
	*this += Chunk(buffer);
	return !_fail;
}