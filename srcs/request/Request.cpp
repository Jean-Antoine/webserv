/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:38:31 by lpaquatt          #+#    #+#             */
/*   Updated: 2025/01/07 11:36:32 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "AMethod.hpp"

Request::Request():
	_method("INVALID"),
	_httpVersion(""),
	_body(""),
	_complete(true),
	_parsingFailed(false)
{
}

Request::~Request()
{
}

Request::Request(const char *buffer, bool isCgiOut):
	_method("INVALID"),
	_httpVersion(""),
	_body(""),
	_complete(true),
	_parsingFailed(false),
	_isCgiOut(isCgiOut)
{
	std::string	bufferString(buffer);

	_bufferLines = split(bufferString, CRLF);
	parseRequest();
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
	_complete= src._complete;
	_parsingFailed = src._parsingFailed;
	_isCgiOut = src._isCgiOut;
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
		return parsingFail("parsing request line");
	_method = pattern[0];
	_uri = URI(pattern[1].c_str());
	_httpVersion = pattern[2];
	return	EXIT_SUCCESS;
}

int	Request::parseHeader(size_t lineIdx)
{
	if (lineIdx + 1 >= _bufferLines.size() && _bufferLines[lineIdx].empty())
		return EXIT_SUCCESS;
	std::istringstream lineStream(_bufferLines[lineIdx]);
	std::string key, value;
	
	std::getline(lineStream, key, ':');
	std::getline(lineStream >> std::ws, value);
	if (lineStream.fail())
		return parsingFail("parsing header");
	_headers[key] = value;
	return EXIT_SUCCESS;
}

int Request::parseBody(size_t lineIdx)
{
	std::string body;

	if (_headers["Transfer-Encoding"] == "chunked")
		return addChunks(lineIdx);
	while (lineIdx < _bufferLines.size())
		_body.append(_bufferLines[lineIdx++] + CRLF);
	return EXIT_SUCCESS;
}

int Request::parseRequest()
{
	if (!_isCgiOut && parseReqLine())
		return EXIT_FAILURE;
	size_t lineIdx = !_isCgiOut;
	while (lineIdx < _bufferLines.size() && !_bufferLines[lineIdx].empty())
		if (parseHeader(lineIdx++))
			return EXIT_FAILURE;
	parseBody(++lineIdx);
	return EXIT_SUCCESS;
}

bool Request::isEndOfChunks(int lineIdx) const
{
	return _bufferLines[lineIdx]== "0"
		&& _bufferLines[lineIdx + 1].empty();
}

bool Request::isValidChunk(size_t lineIdx) const
{
	size_t chunkSize;
	if (convertHexa(_bufferLines[lineIdx], chunkSize)
		|| _bufferLines[lineIdx + 1].size() != chunkSize)
		return false;
	return true;
}

bool Request::isLastLine(size_t lineIdx) const
{
	return lineIdx + 1 == _bufferLines.size() && _bufferLines[lineIdx].empty();
}

int Request::addChunks(size_t lineIdx)
{
	_complete = false;
	if (isLastLine(lineIdx))
		return EXIT_SUCCESS;
	int status = EXIT_SUCCESS;
	while (lineIdx + 1 < _bufferLines.size())
	{
		if (isEndOfChunks(lineIdx))
		{
			_complete = true;
			lineIdx += 2;
			break;
		}
		if (!isValidChunk(lineIdx))
			status = EXIT_FAILURE;
		_body.append(_bufferLines[lineIdx + 1]);
		lineIdx += 2;
	}
	if (status == EXIT_FAILURE || (!isLastLine(lineIdx)))
		return parsingFail("invalid chunked encoding");
	return EXIT_SUCCESS;	
}

int Request::addNewChunks(const char *buffer)
{
	std::string	bufferString(buffer);
	_bufferLines = split(bufferString, CRLF);
	return addChunks(0);
}

std::string	Request::response(Config *config)
{
	AMethod		*method;
	std::string	out;

	if (_method == "POST")
		method = new Post(config, *this);
	else
		method = new Get(config, *this);
	// else
	// 	method = new (config, *this);
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

int Request::parsingFail(const std::string &errorMessage)
{
	_parsingFailed = true;
	Logs(RED) < "Parsing failed: " < errorMessage < "\n";
	return (EXIT_FAILURE);
}

const std::string &	Request::getHeader(const char *key) const
{
	if (_headers.find(key) == _headers.end())
		return empty::string;
	return _headers.find(key)->second;
}
