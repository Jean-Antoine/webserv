/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:38:31 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/02 15:41:50 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "AMethod.hpp"

Request::Request():
	_responseCode(400),
	_method(INVALID),
	_httpVersion("")
{
}

Request::~Request()
{
}

Request::Request(char *buffer):
	_responseCode(400),
	_method(INVALID),
	_httpVersion("")
{
	std::string	bufferString(buffer);
	
	_bufferLines = split(bufferString, CRLF);
	if (_bufferLines.empty())
	{
		setResponseCode(400, "Bad Request (Empty request)");
		return ;
	}
	if (parseRequest())
		return ;
	setResponseCode(200);
}

Request& Request::operator=(const Request & src)
{
	if (this == &src)
		return *this;
	_responseCode = src._responseCode;
	_bufferLines = src._bufferLines;
	_method = src._method;
	_uri = src._uri;
	_httpVersion = src._httpVersion;
	_headers = src._headers;
	_body = src._body;
	return *this;
}

Request::Request(const Request & req)
{
	*this = req;
}

int Request::setResponseCode(int code, std::string message) //avant de gerer mieux les erreurs
{
	_responseCode = code;
	if (code != 200)
		putError(message, code);
	return code;
}

static enum method	strToMethod(std::string method)
{
	if (method == "GET")
		return GET;
	else if (method == "POST")
		return POST;
	else if (method == "DELETE")
		return DELETE;
	return INVALID;
}

int	Request::parseReqLine()
{
	t_stringVector pattern;
	
	pattern = split(_bufferLines[0], " ");
	if  (pattern.size() != 3)
		return setResponseCode(400, "Bad Request (failed to parse Request line)");// manage errors
	_method = strToMethod(pattern[0]);
	_uri = URI(pattern[1].c_str());
	_httpVersion = pattern[2];
	return	0;
}

int	Request::parseHeader(size_t lineIdx)
{
	std::istringstream lineStream(_bufferLines[lineIdx]);
	std::string key, value;
	
	std::getline(lineStream, key, ':');
	std::getline(lineStream >> std::ws, value);
	if (lineStream.fail())
		return setResponseCode(400, "Bad Request (failed to parse Headers)");// manage errors
	_headers[key] = value;
	return 0;
}

int Request::parseBody(size_t lineIdx) //nothing could go wrong ?
{
	while (lineIdx < _bufferLines.size())
		_body.append(_bufferLines[lineIdx++] + CRLF);
	return 0;
}

int Request::parseRequest()
{
	if (parseReqLine())
		return 1;
	size_t lineIdx = 1;
	while (lineIdx < _bufferLines.size() && !_bufferLines[lineIdx].empty())
		if (parseHeader(lineIdx++))
			return 1;
	parseBody(++lineIdx);
	return 0;
}

std::string	Request::response()
{
	AMethod		*method;
	std::string	out;

	switch(_method)
	{
		// case GET: method = new Get(this);
		// 	break; 
		// case POST: method = new Post(this);
		// 	break;
		// case DELETE: method = new Delete(this);
		// 	break;
		default: method = new Invalid(_config, *this);
			break;
	}
	out = method->response();
	delete method;
	return out;
	// std::string	out = "HTTP/1.1 200 OK" CRLF CRLF;
	
	// out.append("<h1> BUILDING RESPONSE </h1>");
	// return out;
}
