/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:38:31 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/11/28 14:06:09 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "Request.hpp"
// #include "logs.hpp"
#include "../include/Request.hpp"
#include "../include/logs.hpp"

Request::Request(char *buffer) : _ResponseCode(400)
{
	parseBufferLines(buffer);
	if (_bufferLines.empty())
		setResponseCode(400, "Bad Request (Empty request)");
	if (parseReqLine(_bufferLines[0]))
		return ;
	size_t i = 1;
	while (i < _bufferLines.size() && !_bufferLines[i].empty())
		if (parseHeader(_bufferLines[i++]))
			return ;
	// parseBody(_bufferLines[++i]);
	// if (!_bufferLines[++i].empty())
	// 	setResponseCode(400, "Bad Request (Invalid request format)");
	setResponseCode(200);
}

Request::Request(Request &req) : _ResponseCode(req._ResponseCode), _body(req._body)
{
	t_stringVector	bufferLines(req.getBufferLines());
	t_reqLine		reqLine = {req.getReqLine().method, req.getReqLine().reqURI, req.getReqLine().httpVersion};
	t_headers		headers(req.getHeaders());
	
	_bufferLines = bufferLines;
	_reqLine = reqLine;
	_headers = headers;
}

int Request::setResponseCode(int code, std::string message)
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

int Request::validateMethod(std::string method)
{
	_reqLine.method = strToMethod(method);
	if (_reqLine.method == INVALID)
		return setResponseCode(501, "Method not implemented");
	return 0;
}

static bool isValidHost(std::string host)
{
	if (host.empty() || host.find("..") != std::string::npos || !std::isalnum(host[0])
		|| (host.rfind('.') != std::string::npos &&  host.rfind('.') >= host.length() - 2))
		return false;
	for (size_t i = 0; i < host.length(); ++i)
	{
		char c = host[i];
		if (!std::isalnum(c) && c != '.' && c != '-')
			return false;
	}
	return true;
}

//accepts absolute path and http(s)://host/path
int Request::validateReqURI(std::string reqURI)
{
	if (reqURI.empty())
		return setResponseCode(400, "Bad Request (URI is empty)");
	for (size_t i = 0; i < reqURI.size(); ++i) {
		char c = reqURI[i];
		if (!(std::isprint(c) && c != ' ' && c != '\t'))
			return setResponseCode(400, "Bad Request (URI contains invalid characters)");
	}
	if (reqURI[0] != '/'
		&& ((reqURI.compare(0, 7, "http://") != 0 && reqURI.compare(0, 8, "https://") != 0)
			|| !isValidHost(reqURI.substr(reqURI.find("://") + 3))))
			return setResponseCode(400, "Bad Request (Invalid URI)");
	_reqLine.reqURI = reqURI;
	return 0;
}

int Request::validateHttpVersion(std::string httpVersion)
{
	if( httpVersion.length() != 8
		|| httpVersion.compare(0, 5, "HTTP/") != 0
		|| !std::isdigit(httpVersion[5])
		|| httpVersion[6] != '.'
		|| !std::isdigit(httpVersion[7]))
		return setResponseCode(400, "Bad request (Invalid Http Version format)");
	if (httpVersion != "HTTP/1.1")
		return setResponseCode(505, "HTTP Version Not Supported");
	_reqLine.httpVersion = httpVersion;
	return 0;
}

int	Request::parseReqLine(std::string reqLine)
{
	std::istringstream streamLine;
	streamLine.str(reqLine);
	
	std::string method, reqURI, httpVersion;
	if (!std::getline(streamLine, method, ' ')
		|| !std::getline(streamLine, reqURI, ' ')
		|| !std::getline(streamLine, httpVersion, '\0'))
		{
			if (streamLine.eof() || streamLine.fail())
				return setResponseCode(400, "Bad Request (failed to parse Request line)");// manage errors
		}
	if (validateMethod(method) || validateReqURI(reqURI) || validateHttpVersion(httpVersion))
		return 1;
	// testLog("ReqLine\n\tmethod: " + method + "\n\treqURI: " + _reqLine.reqURI + "\n\thttpVersion: " + _reqLine.httpVersion);
	return	0;
}

int	Request::parseHeader(std::string header) //pas fini
{
	std::istringstream lineStream(header);
	std::string key, value;
	std::getline(lineStream, key, ':');
	std::getline(lineStream >> std::ws, value);
	if (lineStream.fail())
		return setResponseCode(400, "Bad Request (failed to parse Headers)");// manage errors
	_headers[key] = value;
	// testLog("Header\n\tkey: " + key + "\n\tvalue: " + value);
	return 0;
}

// int Request::parseBody() //traiter differemment selon la methode
// {
// 	std::string contentLength = _headers["Content-Length"];
// 	if (_headers["Content-Length"].empty())
// 		return 0;
// 	if (!std::getline(_bufferStream, _body, '\0'))
// 		return putError("Failed to parse body");
// 	// testLog("Body\n\t" + _body);
// 	return 0;
// }

Request *Request::getReq()
{
	if (_ResponseCode != 200)
		return new Request(*this);
	// switch (_reqLine.method)
	// {
	// 		case GET: return new RequestGet(this); 
	// 		case POST: return new RequestPost(this);
	// 		case DELETE: return new RequestDelete(this);
	// 		default: return new Request(this);
	// }
	switch (_reqLine.method) //test tant que classes enfant pas creees
	{
		case GET: testLog(GREEN + "> Creating GET request");
			break;
		case POST: testLog(GREEN + "> Creating POST request");
			break;
		case DELETE: testLog(GREEN + "> Creating DELETE request");
			break;
		default: testLog(RED + "> Bad method, no child request created");
	}
	return new Request();
}

void	Request::parseBufferLines(char *buffer)
{
	std::string	bufferString(buffer);

	if (bufferString.length() < 2)
	{
		_bufferLines.push_back(bufferString);
		return ;
	}
	size_t endl = bufferString.find("\r\n");
	while (endl != std::string::npos)
	{
		_bufferLines.push_back(bufferString.substr(0, endl));
		bufferString = bufferString.substr(endl + 2);
		endl = bufferString.find("\r\n");
	}
	if (!bufferString.empty())
		_bufferLines.push_back(bufferString);
}



Request *parseRequest(char *buffer)
{
	log("------ BUFFER ------");
	log(buffer);
	log("--------------------\n");

	Request	req(buffer);
	return req.getReq();
}

#include <fcntl.h>
#include <unistd.h>

int main(int ac, char **av)
{
	if (ac != 2)
		return putError("Wrong number of arguments");
	
	int fd = open(av[1], O_RDONLY);
	char buffer[2048];
	size_t bytes = read(fd, buffer, 2048);
	buffer[bytes] = '\0';
	close(fd);
	Request *req = parseRequest(buffer);
	delete req;
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