/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:38:31 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/11/26 08:36:03 by jeada-si         ###   ########.fr       */
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
		return putError("Invalid method");
// Un serveur d’origine DEVRAIT retourner le code d’état 405 (Méthode non admise) si la méthode est connue par le
// serveur d’origine mais non admise pour la ressource demandée, et 501 (Non mis en œuvre) si la méthode n’est
// pas reconnue ou pas mise en œuvre par le serveur d’origine.
	return 0;
}

int Request::validatePath(std::string path)
{
	if (path.empty() || path[0] != '/') //autre choses a verif ?
		putError("Invalid path: " + path);
	_reqLine.path = path;
	return 0;
}

int Request::validateHttpVersion(std::string httpVersion)
{
	if (httpVersion != "HTTP/1.1") // a check
		return putError("Invalid HTTP version"); 
	_reqLine.httpVersion = httpVersion;
	return 0;
}

int	Request::parseReqLine()
{
	std::string method, path, httpVersion;
	if (!std::getline(_bufferStream, method, ' ')
		|| !std::getline(_bufferStream, path, ' ')
		|| !std::getline(_bufferStream, httpVersion, '\n'))
		{
			if (_bufferStream.eof() || _bufferStream.fail())
				return putError("Failed to parse request line");// manage errors
		}
	if (validateMethod(method) || validatePath(path) || validateHttpVersion(httpVersion))
		return 1;
	testLog("ReqLine\n\tmethod: " + method + "\n\tpath: " + _reqLine.path + "\n\thttpVersion: " + _reqLine.httpVersion);
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
		testLog("Header\n\tkey: " + key + "\n\tvalue: " + value);
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
	testLog("Body\n\t" + _body);
	return 0;
}


Request *Request::getReq()
{
	if (!_isValid)
		return new Request();
	// switch (_reqLine.method)
	// {
	// 		case GET: return new RequestGet();
	// 		case POST: return new RequestPost();
	// 		case DELETE: return new RequestDelete();
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

Request *parseRequest(char *buffer)
{
	log("------ BUFFER ------");
	log(buffer);
	log("--------------------\n");
	Request req(buffer);
	if (req.parseReqLine()
		|| req.parseHeaders()
		|| req.parseBody())
		return new Request();
	req.setValid(true);
	return req.getReq();
}

int main()
{
	char buffer[] = "DELETE / HTTP/1.1\nHost: localhost:8080\nUser-Agent: curl/7.68.0\nAccept: */*\n\nThis is the body\n";
	Request *req = parseRequest(buffer);
	delete req;
	return 0;
}
