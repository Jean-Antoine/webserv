/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:38:31 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/11/29 10:05:29 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request():
	_ResponseCode(400),
	_reqLine((t_reqLine){INVALID, "", ""})
{
}

Request::~Request()
{
}

Request::Request(char *buffer):
	_ResponseCode(400),
	_reqLine((t_reqLine){INVALID, "", ""})
{
	std::string	bufferString(buffer);
	_bufferLines = split(bufferString, CRLF);
	if (_bufferLines.empty())
		setResponseCode(400, "Bad Request (Empty request)");
	if (parseRequest())
		return;
	setResponseCode(200);
}

Request& Request::operator=(const Request & src)
{
	if (this == &src)
		return *this;
	_ResponseCode = src._ResponseCode;
	_bufferLines = src._bufferLines;
	_reqLine = src._reqLine;
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
	_ResponseCode = code;
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
	std::istringstream streamLine;
	streamLine.str(_bufferLines[0]);
	
	std::string method, reqURI, httpVersion;
	if (!std::getline(streamLine, method, ' ')
		|| !std::getline(streamLine, reqURI, ' ')
		|| !std::getline(streamLine, httpVersion, '\0'))
		{
			if (streamLine.eof() || streamLine.fail())
				return setResponseCode(400, "Bad Request (failed to parse Request line)");// manage errors
		}
	_reqLine = (t_reqLine){strToMethod(method), reqURI, httpVersion};
	// testLog("Parsing ReqLine\n\tmethod: " + method + "\n\treqURI: " + _reqLine.reqURI + "\n\thttpVersion: " + _reqLine.httpVersion);
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
	// testLog("Parsing Header\n\tkey: " + key + "\n\tvalue: " + value);
	return 0;
}

int Request::parseBody(size_t lineIdx) //nothing could go wrong ?
{
	while (lineIdx < _bufferLines.size())
		_body.append(_bufferLines[lineIdx++] + CRLF);
	// testLog("Parsing Body\n'" + _body + "'");
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
	// AMethod		*method;
	// std::string	out;

	// switch(_reqLine.method)
	// {

	// }
	// out = method->response();
	// delete method;
	// return out;
	std::string	out = "HTTP/1.1 200 OK" CRLF CRLF;
	
	out.append("<h1> BUILDING RESPONSE </h1>");
	return out;
}

// Request *Request::getReq()
// {
// 	if (_ResponseCode != 200)
// 		return new Request(*this);
// 	// switch (_reqLine.method)
// 	// {
// 	// 		case GET: return new RequestGet(this); 
// 	// 		case POST: return new RequestPost(this);
// 	// 		case DELETE: return new RequestDelete(this);
// 	// 		default: return new Request(this);
// 	// }
// 	switch (_reqLine.method) //test tant que classes enfant pas creees
// 	{
// 		case GET: testLog(GREEN "> Creating GET request");
// 			break;
// 		case POST: testLog(GREEN "> Creating POST request");
// 			break;
// 		case DELETE: testLog(GREEN "> Creating DELETE request");
// 			break;
// 		default: testLog(RED "> Bad method, no child request created");
// 	}
// 	return new Request();
// }


// #include <fcntl.h>
// #include <unistd.h>

// int main(int ac, char **av)
// {
// 	if (ac != 2)
// 		return putError("Wrong number of arguments");
	
// 	int fd = open(av[1], O_RDONLY);
// 	char buffer[2048];
// 	size_t bytes = read(fd, buffer, 2048);
// 	buffer[bytes] = '\0';
// 	close(fd);
// 	Request req(buffer);
// 	return 0;
// }

