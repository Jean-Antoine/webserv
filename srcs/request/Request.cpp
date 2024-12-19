/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:38:31 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/19 14:03:10 by jeada-si         ###   ########.fr       */
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
	parseRequest(); //le if n'a aucun sens..? > mettre invalid request ou envoyer une exception
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

	while (lineIdx < _bufferLines.size())
		body.append(_bufferLines[lineIdx++] + CRLF);
	if (_headers["Transfer-Encoding"] == "chunked")
		return addChunk(body.substr(0, body.length() - 2).c_str());
	_body = body;
	return EXIT_SUCCESS;
}

int Request::parseRequest()
{
	if (parseReqLine() && !_isCgiOut)
		return EXIT_FAILURE;
	size_t lineIdx = !_isCgiOut;
	while (lineIdx < _bufferLines.size() && !_bufferLines[lineIdx].empty())
		if (parseHeader(lineIdx++))
			return EXIT_FAILURE;
	parseBody(++lineIdx);
	return EXIT_SUCCESS;
}

bool Request::isEndOfChunks(const char *buffer) const
{
	std::string bufStr = buffer;
	return bufStr.compare(0, 5, "0\r\n\r\n") == 0;
}

bool Request::isCRLF(const char *buffer) const
{
	return buffer[0] == '\r' && buffer[1] == '\n';
}

int Request::readChunkSize(const char *buffer, size_t &chunkSize, size_t &bytesRead)
{
	std::string hexa;
	size_t	i = 0;
	if (isEndOfChunks(&buffer[i]))
		return EXIT_SUCCESS;
	while (buffer[i] && !isCRLF(&buffer[i]))
		hexa += buffer[i++];
	if (!isCRLF(&buffer[i]))
	{
		bytesRead = i - 1;
		return EXIT_FAILURE;
	}
	bytesRead = i + 2;
	return convertHexa(hexa, chunkSize);
}

int Request::appendChunk(const char *buffer,const  size_t &chunkSize, size_t &bytesRead)
{
	size_t i = 0;
	while (buffer[i] && !isCRLF(&buffer[i]))
	{
		if (isEndOfChunks(&buffer[i]))
		{
			bytesRead = i;
			return EXIT_FAILURE;
		}
		_body += buffer[i];
		i++;
	}
	bytesRead = i;
	if (i != chunkSize || !buffer[i])
		return EXIT_FAILURE;
	bytesRead += 2;
	return EXIT_SUCCESS;
}


int Request::addChunk(const char *buffer)
{
	_complete = false;
	if (!buffer || buffer[0] == '\0') //ex: le premier read n'a que les headers et pas de body -> c'est une chunk request donc il faut metter en incomplete mais c'est tout
		return EXIT_SUCCESS;
	int status = EXIT_SUCCESS;
	size_t i = 0;
	while (buffer[i])
	{
		if (isEndOfChunks(&buffer[i]))
		{
			_complete = true;
			if (buffer[i + 5] != '\0')
				status = EXIT_FAILURE;
			break;
		}
		size_t	chunkSize = 0, bytesRead = 0;
		if (readChunkSize(&buffer[i], chunkSize, bytesRead))
			status = EXIT_FAILURE;
		i += bytesRead;
		bytesRead = 0;
		if (appendChunk(&buffer[i], chunkSize, bytesRead))
			status = EXIT_FAILURE;
		i += bytesRead;
	}
	if (status == EXIT_FAILURE)
		return parsingFail("invalid chunked encoding");
	return EXIT_SUCCESS;
}

std::string	Request::response(Config *config)
{
	AMethod		*method;
	std::string	out;

	// if (_method == "GET")
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
