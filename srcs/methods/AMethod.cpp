/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 19:21:14 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/02 19:33:24 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMethod.hpp"

AMethod::AMethod(Config & config, Request & request):
	_reqLine(request.getReqLine()),
	_headers(request.getHeaders()),
	_body(request.getBody()),
	_config(config)
{
	_response.statusLine =(t_statusLine){"HTTP/1.1", 200, "OK"};
	_response.headers["Date"] = getDate();
	_response.headers["Connection"] = "keep-alive"; // ou "close" mais je sais pas comment je le sais
	// _response.headers["Connection"] = "close"; // -> client renvoie une request vide et la connection se ferme
}

int AMethod::setResponseCode(int code, std::string message) //avant de gerer mieux les erreurs
{
	_response.statusLine.code = code;
	_response.statusLine.reasonPhrase = message; //todo attention aux mauvais messages pour l'instant, faire un dictionnaire selon le code ..?
	if (code != 200)
		putError(message, code);
	return code;
}

bool AMethod::isValid()
{
	if (validateReqURI()
		|| validateMethod()
		|| validateHttpVersion())
		return false;
	else return true;
}

int AMethod::validateMethod() // + verifier selon la config
{
	if (_reqLine.method == INVALID)
		return setResponseCode(501, "Method not implemented");
	// if (!_config.isMethodAllowed(_reqLine.reqURI, _reqLine.method)) // a implementer
	if (_reqLine.method == GET) // pour test
	{
		_response.headers["Allow"] = "POST";
		return setResponseCode(405, "Method not allowed");
	}
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
//atttion > pas possible de mettre un port..?
int AMethod::validateReqURI()
{
	std::string	reqURI = _reqLine.reqURI;
	if (reqURI.empty())
		return 400; //setResponseCode(400, "Bad Request (URI is empty)");
	for (size_t i = 0; i < reqURI.size(); ++i) {
		char c = reqURI[i];
		if (!(std::isprint(c) && c != ' ' && c != '\t'))
			return 400; //setResponseCode(400, "Bad Request (URI contains invalid characters)");
	}
	if (reqURI[0] != '/'
		&& ((reqURI.compare(0, 7, "http://") != 0 && reqURI.compare(0, 8, "https://") != 0)
			|| !isValidHost(reqURI.substr(reqURI.find("://") + 3))))
			return 400; //setResponseCode(400, "Bad Request (Invalid URI)");
	return 0;
}

int AMethod::validateHttpVersion()
{
	std::string httpVersion = _reqLine.httpVersion;
	if( httpVersion.length() != 8
		|| httpVersion.compare(0, 5, "HTTP/") != 0
		|| !std::isdigit(httpVersion[5])
		|| httpVersion[6] != '.'
		|| !std::isdigit(httpVersion[7]))
		return 400; // setResponseCode(400, "Bad request (Invalid Http Version format)");
	if (httpVersion != "HTTP/1.1")
		return 505; // setResponseCode(505, "HTTP Version Not Supported");
	return 0;
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
		return "";
	}
	_response.body = getErrorBody(_response.statusLine.code);
	_response.headers["Content-Type"] = "text/html; charset=UTF-8";
	_response.headers["Content-Length"] = to_string(_response.body.size());
	return buildResponse();
}

std::string	AMethod::buildResponse()
{
	std::ostringstream responseStream;

	//verifier si la reponse est valide ?? 
	responseStream << _response.statusLine.httpVersion << " "
				<< _response.statusLine.code << " "
				<< _response.statusLine.reasonPhrase << "\r\n";
	if (_response.headers.find("Content-Length") == _response.headers.end())
		_response.headers["Content-Length"] = to_string(_response.body.size());

	for (t_headers::const_iterator it = _response.headers.begin(); it != _response.headers.end(); ++it) 
		responseStream << it->first << ": " << it->second << "\r\n";

	responseStream << "\r\n";
	responseStream << _response.body;

	return responseStream.str();
}
