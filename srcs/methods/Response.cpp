/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 10:13:44 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/19 14:53:13 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Response.hpp"

t_statusMap Response::_statusCodes = Response::initPhrases();

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

int Response::setResponseCode(int code, std::string message)
{
	_code = code;
	_reasonPhrase = getReasonPhrase(code);
	if (code == 200)
		return true;
	Logs(RED) < _reasonPhrase < ": " < message < "\n";
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
		setResponseCode(500, "Opening error file");
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

const std::string&	Response::getReasonPhrase(int statusCode)
{
	t_statusMap::const_iterator it = _statusCodes.find(statusCode);
	if (it != _statusCodes.end()) {
		return it->second;
	}
	static const std::string unknown = "Unknown Status Code";
	return unknown;
}

t_statusMap Response::initPhrases()
{
	t_statusMap tmp;

	// Success (2xx)
	tmp[200] = "OK";
	tmp[201] = "Created";
	tmp[202] = "Accepted";
	tmp[203] = "Non-Authoritative Information";
	tmp[204] = "No Content";
	tmp[205] = "Reset Content";
	tmp[206] = "Partial Content";

	// Redirection (3xx)
	tmp[300] = "Multiple Choices";
	tmp[301] = "Moved Permanently";
	tmp[302] = "Found";
	tmp[303] = "See Other";
	tmp[304] = "Not Modified";
	tmp[305] = "Use Proxy";
	tmp[307] = "Temporary Redirect";

	// Client Errors (4xx)
	tmp[400] = "Bad Request";
	tmp[401] = "Unauthorized";
	tmp[402] = "Payment Required";
	tmp[403] = "Forbidden";
	tmp[404] = "Not Found";
	tmp[405] = "Method Not Allowed";
	tmp[406] = "Not Acceptable";
	tmp[407] = "Proxy Authentication Required";
	tmp[408] = "Request Timeout";
	tmp[409] = "Conflict";
	tmp[410] = "Gone";
	tmp[411] = "Length Required";
	tmp[412] = "Precondition Failed";
	tmp[413] = "Payload Too Large";
	tmp[414] = "URI Too Long";
	tmp[415] = "Unsupported Media Type";
	tmp[416] = "Range Not Satisfiable";
	tmp[417] = "Expectation Failed";
	tmp[418] = "I'm a teapot"; // Easter egg from RFC 2324

	// Server Errors (5xx)
	tmp[500] = "Internal Server Error";
	tmp[501] = "Not Implemented";
	tmp[502] = "Bad Gateway";
	tmp[503] = "Service Unavailable";
	tmp[504] = "Gateway Timeout";
	tmp[505] = "HTTP Version Not Supported";
	return  tmp;
}
