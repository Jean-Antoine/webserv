/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 19:21:14 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/12 12:36:58 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMethod.hpp"

AMethod::AMethod(Config *config, Request & request):
	_config(config),
	_request(request)
{
	_route = _config->getRoute(request.getURI());
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
	if (!validateURI()
		|| !validateRoute()
		|| !validateHttpVersion()
		|| !validateMethod())
		return false;
	return true;
}

bool	AMethod::checkAllowedMethods()
{	
	if (_route.isMethodAllowed(_request.getMethod()))
		return true;
	setResponseCode(405, "Method Not Allowed");
	_response.headers["Allow"] = concatStrVec(_route.getAllowedMethods(), ", ", true);
	return false;	
}

bool	AMethod::validateMethod()
{
	if (_request.getMethod() == "INVALID")
		return setResponseCode(501, "Method not implemented");
	return checkAllowedMethods();
}

bool	AMethod::validateURI()
{
	if (_request.getURI().bad())
		return setResponseCode(400, "Bad URI");
	return true;
}

bool	AMethod::validateRoute()
{
	if (_route.bad())
		return setResponseCode(404, "Not found");
	return true;
}

bool	AMethod::validateHttpVersion()
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

std::string AMethod::getErrorBody(int code)
{
	if (code == 204)
		return "";
	const std::string errorDir = "./default_errors/";
	std::ostringstream filePath;
	filePath << errorDir << code << ".html";
	
	std::string body;
	if (readFile(filePath.str(), body) == EXIT_FAILURE)
	{
		setResponseCode(500, "Internal Server Error");
		return "<html><h1>500 Internal Server Error</h1></html>";
	}
	return body;
}

//todo : gerer les messages selon la config
std::string	AMethod::errorResponse()
{
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
	testLog(responseStream.str()); //pour images
	responseStream << _response.body;
	responseStream << CRLF;

	return responseStream.str();
}

std::string AMethod::getMimeType(const std::string & path)
{
	std::string	extension;

	size_t dotPos = path.find_last_of('.');
	if (dotPos != std::string::npos)
		extension = path.substr(dotPos + 1);
	return _config->getMimeType(extension);
}
