/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 10:13:44 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/28 07:18:25 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Response.hpp"
# include "Logs.hpp"
# include "Ressource.hpp"

t_statusMap Response::_statusCodes = Response::initPhrases();

Response::Response():
	_httpVersion("HTTP/1.1"),
	_code(200),
	_reasonPhrase("OK"),
	_cgi(NULL)
{
	setHeader("Date", getDate());
	setHeader("Connection", "keep-alive");
}

Response::Response(bool timeout):
	_httpVersion("HTTP/1.1"),
	_code(200),
	_reasonPhrase("OK"),
	_cgi(NULL)
{
	setHeader("Date", getDate());
	if (timeout)
	{
		setHeader("Connection", "close");
		setResponseCode(408, "Request timeout");
	}
}

Response::Response(const Response &src)
{
	*this = src;
}

Response& Response::operator=(const Response &src)
{
	if (this == &src)
		return *this;
	Message::operator=(src);
	_httpVersion = src._httpVersion;
	_code = src._code;
	_reasonPhrase = src._reasonPhrase;
	_statusCodes = src._statusCodes;
	_cgi = src._cgi;
	return *this;
}

Response::~Response()
{
}

void	Response::runCGI(const Request & request,
	const std::string & scriptPath,
	const std::string & binPath)
{
	
	_cgi = new CGI(request,	scriptPath,	binPath);
	if (_cgi->run())
	{
		delete _cgi;
		_cgi = NULL;
		setResponseCode(500, "Error while executing CGI");
	}
}

void	Response::setResponseCode(int code, std::string message)
{
	_code = code;
	_reasonPhrase = getReasonPhrase(code);
	const char *color = (code >= 400) ? RED : GREEN;
	Logs(color) < code < " " < _reasonPhrase < ": " < message < "\n";
}

void	Response::setError(Config &config)
{
	Ressource	errorFile(config.getErrorPage(_code, false));
	setHeader("Content-Type", "text/html; charset=UTF-8");
	if (_code == 204)
		return setBody("");

	if (!errorFile.getPath().exist() || errorFile.readFile())
		errorFile = Ressource(config.getErrorPage(_code, true));
	if (errorFile.readFile())
	{
		setResponseCode(500, "Opening error file " + errorFile.getPath().litteral());
		setBody("<html><h1>500 Internal Server Error</h1></html>");
	}
	else 
		setBody(errorFile.fileContent());
	if (_code == 400)
		setHeader("Connection", "close");
}

static std::string getSetCookieStr(const  t_cookie &cookie)
{
	std::string str = "Set-Cookie: ";
	str += cookie._name + "=" + cookie._value + ";";
	if (cookie._httpOnly)
		str += "HttpOnly;";
	if (cookie._maxAge != 0)
		str += "Max-Age=" + to_string(cookie._maxAge) + ";";
	if (cookie._path != "")
		str += "Path=" + cookie._path + ";";
	if (cookie._domain != "")
		str += "Domain=" + cookie._domain + ";";
	return str;
}

bool	Response::ready()
{
	if (_cgi == NULL)
		return true;
	if (_cgi->execTimeout())
	{
		Logs(RED) << "CGI timeout - sending SIGKILL\n";
		_cgi->killChild();
		setResponseCode(504, "CGI timeout");
	}
	else if (_cgi->ioFailed())
	{
		_cgi->killChild();
		setResponseCode(500, "I/O error on CGI pipe");
	}
	else if (_cgi->execFailed())
		setResponseCode(500, "CGI execution failed");
	else if (_cgi->execComplete() && _cgi->readComplete())
		*this += _cgi->getResult();
	else
		return false;
	return true;
}

CGI *	Response::getCGI() const
{
	return _cgi;
}

void	Response::clearCGI()
{
	if (_cgi == NULL)
		return ;
	delete _cgi;
	_cgi = NULL;
}

std::string	Response::getResponse(Config &config)
{
	std::ostringstream responseStream;
	
	if (_code >= 400)
		setError(config);
	responseStream << "HTTP/1.1 " << _code << " " << _reasonPhrase << CRLF;
	setHeader("Content-Length", to_string(_body.size()));
	for (t_headers::const_iterator it = _headers.begin();
		it != _headers.end(); ++it)
		responseStream << it->first << ": "
			<< it->second << CRLF;
	for (t_cookies::const_iterator it = _cookies.begin();
		it != _cookies.end(); ++it)
		responseStream << getSetCookieStr(it->second) << CRLF;
	responseStream << CRLF;
	if (_body.size())
		responseStream << _body;
	// responseStream << CRLF;
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
