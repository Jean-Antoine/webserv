/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:38:31 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/11/27 14:16:57 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "logs.hpp"

Request::Request(char *buffer) : _isValid(false)
{
	_bufferStream.str(buffer);
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
		return putError("Method not implemented", 501);
	return 0;
}


bool isValidHost(std::string host)
{
	if (host.empty() || host.find("..") != std::string::npos || !std::isalnum(host[0]) || host.rfind('.') >= host.length() - 2)
		return false;
	testLog("test1");
	for (size_t i = 0; i < host.length(); ++i)
	{
		char c = host[i];
		if (!std::isalnum(c) && c != '.' && c != '-')
			return false;
	}
	return true;
}

int Request::validateReqURI(std::string reqURI)
{
	if (reqURI.empty())
		return putError("Bad Request (URI is empty)", 400);
	for (size_t i = 0; i < reqURI.size(); ++i) {
		char c = reqURI[i];
		if (!(std::isprint(c) && c != ' ' && c != '\t'))
			return putError("Bad RequesT (URI contains invalid characters)", 400);
	}
	if (reqURI[0] != '/'
		&& (reqURI.find("://") == std::string::npos
			|| !isValidHost(reqURI.substr(reqURI.find("://") + 3))))
		return putError("Bad Request (Invalid URI)", 400);
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
		return putError("Bad request (Invalid Http Version format)", 400);		
	if (httpVersion != "HTTP/1.1")
		return putError("HTTP Version Not Supported", 505);
	_reqLine.httpVersion = httpVersion;
	return 0;
}

int	Request::parseReqLine()
{
	std::string method, reqURI, httpVersion;
	if (!std::getline(_bufferStream, method, ' ')
		|| !std::getline(_bufferStream, reqURI, ' ')
		|| !std::getline(_bufferStream, httpVersion, '\n'))
		{
			if (_bufferStream.eof() || _bufferStream.fail())
				return putError("Failed to parse request line");// manage errors
		}
	if (validateMethod(method) || validateReqURI(reqURI) || validateHttpVersion(httpVersion))
		return 1;
	// testLog("ReqLine\n\tmethod: " + method + "\n\treqURI: " + _reqLine.reqURI + "\n\thttpVersion: " + _reqLine.httpVersion);
	return	0;
}

int	Request::parseHeaders() // c'est trop moche :(
{
	std::string line;
	while (std::getline(_bufferStream, line))
	{
		if (line.empty())
			break;
		std::istringstream lineStream(line);
		std::string key, value;
		std::getline(lineStream, key, ':');
		std::getline(lineStream >> std::ws, value);
		if (lineStream.fail())
			return putError("Failed to parse headers");
		_headers[key] = value;
		// testLog("Header\n\tkey: " + key + "\n\tvalue: " + value);
	}
	return 0;
}

int Request::parseBody() //traiter differemment selon la methode
{
	std::string contentLength = _headers["Content-Length"];
	if (_headers["Content-Length"].empty())
		return 0;
	if (!std::getline(_bufferStream, _body, '\0'))
		return putError("Failed to parse body");
	// testLog("Body\n\t" + _body);
	return 0;
}

Request *Request::getReq()
{
	if (!_isValid)
		return new Request();
	// switch (_reqLine.method)
	// {
	// 		case GET: return new RequestGet(this); 
	// 		case POST: return new RequestPost(this);
	// 		case DELETE: return new RequestDelete(this);
	// 		default: return new Request();
	// }
	switch (_reqLine.method) //test tant que classes enfant pas creees
	{
		case GET: testLog("> GET request");
			break;
		case POST: testLog("> POST request");
			break;
		case DELETE: testLog("> DELETE request");
			break;
		default: testLog("> Bad method, no child request created");
	}
	return new Request();
}

// std::vector<std::string>	&parselines(char *buffer)
// {
// 	std::string 				bufferString(buffer);
// 	std::vector<std::string>	*lines = new std::vector<std::string>;

// 	if (bufferString.length() < 2)
// 	{
// 		lines->push_back(bufferString);
// 		return *lines;
// 	}
// 	for (int i = 0; bufferString[i]; i++)
// 		;
// }

Request *parseRequest(char *buffer)
{
	log("------ BUFFER ------");
	log(buffer);
	log("--------------------\n");

	Request						req(buffer);
	// std::vector<std::string>	lines = parselines(buffer);
	if (req.parseReqLine()
		|| req.parseHeaders()
		|| req.parseBody())
		return new Request();
	req.setValid(true);
	return req.getReq();
}

int main(int ac, char **av)
{
	// if (ac != 2)
	// 	return putError("Wrong nuber of arguments");
		
	char buffer[] = "DELETE http://localhost:8080 HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/7.68.0\r\nAccept: */*\r\nThis is the body";
	Request *req = parseRequest(buffer);
	delete req;
	return 0;
}
