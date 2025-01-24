/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 19:21:14 by lpaquatt          #+#    #+#             */
/*   Updated: 2025/01/24 16:29:09 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMethod.hpp"
#include "CGI.hpp"

AMethod::AMethod(Config& config, Request & request):
	_config(config),
	_request(request)
{
	Path	path = _request.getURI().getPath();
	
	_route = _config.getRoute(_request.getURI());
	if (path.replacePrefix(_route.getPath(), _route.getRoot()))
		return ;
	_ressource = Ressource(path);
	if (_request.getHeader("Connection") == "close")
		_response.setHeader("Connection", "close");
}

bool AMethod::isValid()
{
	// if (!_request.complete())
	// {
	// 	_response.setResponseCode(408, "Request timeout");
	// 	return false;
	// }
	if (_request.fail())
	{
		_response.setResponseCode(400, "Parsing failed");
		return false;		
	}
	if (!validateURI()
		|| !validateRoute()
		|| !validateHttpVersion()
		|| !validateMethod()
		|| !validatePayLoad())
		return false;
	if (_ressource.forbidden())
	{
		_response.setResponseCode(403, "cannot go thru parent");
		return false;
	}
	// if (!_ressource.getPath().exist())
	// 	_response.setResponseCode(404, "does not exist");
	return true;
}

bool	AMethod::checkAllowedMethods()
{	
	if (_route.isMethodAllowed(_request.getMethod()))
		return true;
	_response.setResponseCode(405, _request.getMethod());
	_response.setHeader(
		"Allow", 
		concatStrVec(_route.getAllowedMethods(), ", ", true));
	return false;	
}

bool	AMethod::validateMethod()
{
	if (_request.getMethod() != "GET"
		&& _request.getMethod() != "POST"
		&& _request.getMethod() != "DELETE")
		{
			_response.setResponseCode(501,
				"Implemented methods are GET, POST and DELETE");
			return false;
		}			
	return checkAllowedMethods();
}

bool	AMethod::validateURI()
{
	if (_request.getURI().bad())
	{
		_response.setResponseCode(400, "Bad URI");
		return false;
	}
	return true;
}

bool	AMethod::validateRoute()
{
	if (_route.empty())
	{
		_response.setResponseCode(404, "Route not found");
		return false;
	}
	return true;
}

bool	AMethod::validateHttpVersion()
{
	const std::string &	httpVersion = _request.getHttpVersion();

	if( httpVersion.length() != 8
		|| httpVersion.compare(0, 5, "HTTP/") != 0
		|| !std::isdigit(httpVersion[5])
		|| httpVersion[6] != '.'
		|| !std::isdigit(httpVersion[7]))
		{
			_response.setResponseCode(400,
				"Invalid Http Version format");
			return false;
		}		
	if (httpVersion != "HTTP/1.1")
	{
		_response.setHeader("Connection", "close");
		_response.setResponseCode(505, httpVersion);
		return false;
	}
	return true;
}

bool	AMethod::validatePayLoad()
{
	if (!_request.isHeaderSet("Content-Length"))
		return true;

	char	*end;
	long	contentLength = std::strtol(_request.getHeader("Content-Length").c_str(), &end, 10);
	if (*end != '\0')
	{
		_response.setResponseCode(400, "Bad content length format");
		return false;
	}
	if (contentLength <= _config.getMaxBodySize())
		return true;
	_response.setResponseCode(413, "Payload too large");
	return false;
}

std::string AMethod::getMimeType()
{
	return _config.getMimeType(_ressource.getExtension());
}

int	AMethod::executeCgi()
{
	std::string	ext = _ressource.getExtension();

	CGI	cgi(_request,
		_ressource.getPath().litteral(),
		_route.getCgiBinPath(ext.c_str())
	);
	if (cgi.execute())
	{
		_response.setResponseCode(cgi.getResponseCode(), "error in CGI execution");
		return EXIT_FAILURE;
	}
	_response += cgi.get();
	return EXIT_SUCCESS;
}

const Response&	AMethod::getResponse()
{	
	if (!isValid())
		return _response;
	setResponse();
	return _response;
}
