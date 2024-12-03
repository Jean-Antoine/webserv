/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 19:21:14 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/03 16:13:08 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMethod.hpp"

AMethod::AMethod(Config & config, Request & request):
	_config(config),
	_route(_config.getRoute(request.getURI())),
	_request(request)
{
	_response.statusLine =(t_statusLine){"HTTP/1.1", 200, "OK"};
	_response.headers["Date"] = getDate();
	if (_request.getHeaders()["Connection"] == "close")
		_response.headers["Connection"] ="close";
	else 
		_response.headers["Connection"] = "keep-alive";
}


int AMethod::setResponseCode(int code, std::string message) //avant de gerer mieux les erreurs
{
	_response.statusLine.code = code;
	_response.statusLine.reasonPhrase = message; //todo attention aux mauvais messages pour l'instant, faire un dictionnaire selon le code ..?
	if (code == 200)
		return true;
	putError(message, code);
	return false;
}

bool AMethod::isValid()
{	
	if (!validateReqURI()
		|| !validateHttpVersion()
		|| !validateMethod())
		return false;
	else return true;
}

static std::string allowedMethodsHeader(t_strMethods &allowedMethods)
{
	std::string header;
	for (int i = 0; i < 3; i++)
	{
		header.append(allowedMethods[i]);
		if (i < 2 && !allowedMethods[i + 1].empty())
			header.append(", ");
	}
	return header;
}

bool AMethod::checkAllowedMethods()
{
	// t_strMethods allowedMethods = _route.getAllowedMethods();
	
	// test
	t_strMethods	allowedMethods = {"GET", "", ""};
	
	if (!allowedMethods[_request.getMethod()].empty())
		return true;
	setResponseCode(405, "Method Not Allowed");
	_response.headers["Allow"] = allowedMethodsHeader(allowedMethods);
	return false;
	
}

bool AMethod::validateMethod()
{
	if (_request.getMethod() == INVALID)
		return setResponseCode(501, "Method not implemented");
	return checkAllowedMethods();
}

// static bool isValidHost(std::string host)
// {
// 	// if (host.empty() || host.find("..") != std::string::npos || !std::isalnum(host[0])
// 	// 	|| (host.rfind('.') != std::string::npos &&  host.rfind('.') >= host.length() - 2))
// 	// 	return false;
// 	// for (size_t i = 0; i < host.length(); ++i)
// 	// {
// 	// 	char c = host[i];
// 	// 	if (!std::isalnum(c) && c != '.' && c != '-')
// 	// 		return false;
// 	// }
// 	return true;
// }


//TBD WITH CLASS URI
//accepts absolute path and http(s)://host/path 
//atttion > pas possible de mettre un port..?
bool AMethod::validateReqURI()
{
	// std::string	reqURI = _reqLine.reqURI;
	// if (reqURI.empty())
	// 	return 400; //setResponseCode(400, "Bad Request (URI is empty)");
	// for (size_t i = 0; i < reqURI.size(); ++i) {
	// 	char c = reqURI[i];
	// 	if (!(std::isprint(c) && c != ' ' && c != '\t'))
	// 		return 400; //setResponseCode(400, "Bad Request (URI contains invalid characters)");
	// }
	// if (reqURI[0] != '/'
	// 	&& ((reqURI.compare(0, 7, "http://") != 0 && reqURI.compare(0, 8, "https://") != 0)
	// 		|| !isValidHost(reqURI.substr(reqURI.find("://") + 3))))
	// 		return 400; //setResponseCode(400, "Bad Request (Invalid URI)");
	return true;
}

bool AMethod::validateHttpVersion()
{
	std::string &	httpVersion = _request.getHttpVersion();
	
	if( httpVersion.length() != 8
		|| httpVersion.compare(0, 5, "HTTP/") != 0
		|| !std::isdigit(httpVersion[5])
		|| httpVersion[6] != '.'
		|| !std::isdigit(httpVersion[7]))
		return setResponseCode(400, "Bad request (Invalid Http Version format)");
	if (httpVersion != "HTTP/1.1")
	{
		_response.headers["Connection"] = "close";
		return setResponseCode(505, "HTTP Version Not Supported");
	}
	return true;
}

static	std::string getErrorBody(int code)
{
	const std::string errorDir = "./default_errors/";
	std::ostringstream filePath;
	filePath << errorDir << code << ".html";
	std::ifstream file(filePath.str().c_str());
	if (!file.is_open()) {
		throw std::runtime_error("Error: Cannot open error response file: " + filePath.str()); //todo: error management ?
	}
	std::ostringstream content;
	content << file.rdbuf();
	file.close();
	return content.str();
}

//todo : gerer les messages selon la config
std::string	AMethod::errorResponse()
{
	if (_response.statusLine.code != 400
		&& _response.statusLine.code != 405
		&& _response.statusLine.code != 501)
	{
		putError("errorResponse: unknown error code"); // todo: changer la verif.. faire un dictionnaire des erreurs..?
		_response.body = "Error: unknown error code";
		return buildResponse();
	}
	_response.body = getErrorBody(_response.statusLine.code);
	_response.headers["Content-Type"] = "text/html; charset=UTF-8";
	// _response.headers["Content-Length"] = to_string(_response.body.size());
	return buildResponse();
}

std::string	AMethod::buildResponse()
{
	std::ostringstream responseStream;

	//verifier si la reponse est valide ?? 
	responseStream << "HTTP/1.1 "
				<< _response.statusLine.code << " "
				<< _response.statusLine.reasonPhrase << CRLF;
	// if (_response.headers.find("Content-Length") == _response.headers.end())
	_response.headers["Content-Length"] = to_string(_response.body.size());

	for (t_headers::const_iterator it = _response.headers.begin(); it != _response.headers.end(); ++it) 
		responseStream << it->first << ": " << it->second << CRLF;

	responseStream << CRLF;
	responseStream << _response.body;
	responseStream << CRLF;

	return responseStream.str();
}
