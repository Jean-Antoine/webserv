/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 10:13:44 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/19 09:29:10 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Response.hpp"

Response::Response():
	_httpVersion("HTTP/1.1"),
	_code(200),
	_reasonPhrase("OK")
{
	setHeader("Date", getDate());
	setHeader("Connection", "keep-alive");
}

Response::Response(const Response &src)
{
	*this = src;
}

Response& Response::operator=(const Response &src)
{
	_httpVersion = src._httpVersion;
	_code = src._code;
	_headers = src._headers;
	_body = src._body;
	_reasonPhrase = src._reasonPhrase;
	return *this;
}

Response::~Response()
{
}

int Response::setResponseCode(int code, std::string message) //avant de gerer mieux les erreurs
{
	_code = code;
	_reasonPhrase = message; //todo attention aux mauvais messages pour l'instant, faire un dictionnaire selon le code ..?
	if (code == 200)
		return true;
	putError(message, code);
	return false;
}

void	Response::setHeader(std::string key, std::string value)
{
	_headers[key] = value;
}

void	Response::appendHeader(std::string key, std::string value)
{
	if (_headers.find(key) != _headers.end())
	{
		_headers[key].append(", ");
		_headers[key].append(value);
	}
	else
		setHeader(key, value);
}

void	Response::setBody(const char *str)
{
	_body = str;
}

void	Response::setBody(std::string str)
{
	_body = str;
}

void	Response::setError()
{
	setHeader("Content-Type", "text/html; charset=UTF-8");
	if (_code == 204)
		return setBody("");
	std::ostringstream filePath;

	filePath << ERROR_DIR << _code << ".html";
	if (readFile(filePath.str(), _body) == EXIT_FAILURE)
	{
		setResponseCode(500, "Internal Server Error");
		setBody("<html><h1>500 Internal Server Error</h1></html>");
	}
}

std::string	Response::getResponse()
{
	std::ostringstream responseStream;

	if (_code >= 400)
		setError();
	//verifier si la reponse est valide ?? 
	responseStream << "HTTP/1.1 "
				<< _code << " "
				<< _reasonPhrase << CRLF;
	setHeader("Content-Length", to_string(_body.size()));
	for (t_headers::const_iterator it = _headers.begin();
		it != _headers.end(); ++it) 
		responseStream << it->first << ": " << it->second << CRLF;

	responseStream << CRLF;
	responseStream << _body;
	responseStream << CRLF;

	return responseStream.str();
}