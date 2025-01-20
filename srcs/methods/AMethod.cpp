/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 19:21:14 by lpaquatt          #+#    #+#             */
/*   Updated: 2025/01/20 13:10:23 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMethod.hpp"
#include "CGI.hpp"

AMethod::AMethod(Config& config, Request & request):
	_config(config),
	_request(request)
{
	_route = _config.getRoute(_request.getURI());
	_ressource = Ressource(_route.getRoot(), _request.getURI().getPath(), _route.getDefaultFile());
	if (_request.getHeader("Connection") == "Close")
		_response.setHeader("Connection", "close");
}

bool AMethod::isValid()
{
	// if (_request.isTLS())
	// {
	// 	_response.setResponseCode(400, "TLS connection not handled by server");
	// 	_response.setHeader("Connection", "Close");
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

bool	AMethod::validatePayLoad() //@leontinepaq should be done in POST method + check each chunk ?
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
	
	if (!_route.isCgiEnabled(ext.c_str()))
	{
		_response.setResponseCode(501, "cgi not enabled");
		return EXIT_FAILURE;// todo @leontinepaq relechir si c'est vraiment un fail ..? / a quoi servent les return ?	
	}

	CGI	cgi(_request,
		_ressource.getPath().litteral(),
		_route.getCgiBinPath(ext.c_str())
	);
	if (cgi.execute())
	{
		_response.setResponseCode(500, "Internal error");
		return EXIT_FAILURE;
	}
	_response += cgi.get();
	return EXIT_SUCCESS;
}

const Response&	AMethod::getResponse()
{
	if (!isValid())
		return _response;
	setResponse(); // @Jean-Antoine pourquoi setResponse renvoie un int si c'est toujours SUCCESS et qu'on l'utilise pas ?
	return _response;
}