/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:38:31 by lpaquatt          #+#    #+#             */
/*   Updated: 2025/01/16 15:04:46 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "AMethod.hpp"

Request::Request():
	_method("INVALID"),
	_httpVersion("")
{	
}

Request::~Request()
{
}

Request::Request(const char *buffer):
	Message(buffer, true),
	_method("INVALID"),
	_httpVersion("")
{
	t_lines	lines = split< t_lines >(buffer, CRLF);
	
	if (_fail)
		return ;
	if (parseReqLine(lines[0]))
		_fail = true;
	// if (!getHeader("Content-Length").empty()
	// && )
}

Request& Request::operator=(const Request & src)
{
	if (this == &src)
		return *this;
	Message::operator=(src);
	_method = src._method;
	_uri = src._uri;
	_httpVersion = src._httpVersion;
	return *this;
}

Request::Request(const Request & req)
{
	*this = req;
}

int	Request::parseReqLine(std::string &line)
{
	t_strVec pattern  = split<t_strVec>(line, " ");
	
	if  (pattern.size() != 3)
		return EXIT_FAILURE;
	_method = pattern[0];
	_uri = pattern[1].c_str();
	_httpVersion = pattern[2];
	return	EXIT_SUCCESS;
}

const std::string&		Request::getMethod() const
{
	return _method;
}

const URI&			Request::getURI() const
{
	return _uri;
}

const std::string&	Request::getHttpVersion() const
{
	return _httpVersion;
}
