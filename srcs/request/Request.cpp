/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:38:31 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/10 14:53:22 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "AMethod.hpp"

Request::Request():
	_method("INVALID"),
	_httpVersion(""),
	_body("")
{
}

Request::~Request()
{
}

Request::Request(char *buffer):
	_method("INVALID"),
	_httpVersion(""),
	_body("")
{
	std::string	bufferString(buffer);
	
	_bufferLines = split(bufferString, CRLF);
	if (parseRequest())
		return ;
}

Request& Request::operator=(const Request & src)
{
	if (this == &src)
		return *this;
	_bufferLines = src._bufferLines;
	_method = src._method;
	_uri = src._uri;
	_httpVersion = src._httpVersion;
	_headers = src._headers;
	_body = src._body;
	_path = src._path; //test avant uri
	return *this;
}

Request::Request(const Request & req)
{
	*this = req;
}

int	Request::parseReqLine()
{
	t_strVec pattern;
	
	pattern = split(_bufferLines[0], " ");
	if  (pattern.size() != 3)
		return EXIT_FAILURE;
	_method = pattern[0];
	_uri = URI(pattern[1].c_str());
	_httpVersion = pattern[2];
	return	EXIT_SUCCESS;
}

int	Request::parseHeader(size_t lineIdx)
{
	std::istringstream lineStream(_bufferLines[lineIdx]);
	std::string key, value;
	
	std::getline(lineStream, key, ':');
	std::getline(lineStream >> std::ws, value);
	if (lineStream.fail())
		return  EXIT_FAILURE;
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
		return EXIT_FAILURE;
	size_t lineIdx = 1;
	while (lineIdx < _bufferLines.size() && !_bufferLines[lineIdx].empty())
		if (parseHeader(lineIdx++))
			return EXIT_FAILURE;
	parseBody(++lineIdx);
	return EXIT_SUCCESS;
}

std::string	Request::response(Config *config)
{
	AMethod		*method;
	std::string	out;

	if (_method == "GET")
		method = new Get(config, *this);
	else
		method = new Invalid(config, *this);
	out = method->getResponse();
	delete method;
	return out;
}

bool	Request::keepAlive()
{
	if (_headers.find("Connection") == _headers.end())
		return true;
	return _headers["Connection"].compare("close");
}
