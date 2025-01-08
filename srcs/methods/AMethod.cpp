/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 19:21:14 by lpaquatt          #+#    #+#             */
/*   Updated: 2025/01/08 11:57:20 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMethod.hpp"

AMethod::AMethod(Config *config, Request & request):
	_config(config),
	_request(request)
{
	_route = _config->getRoute(_request.getURI());
	_ressource = Ressource(_route, _request.getURI().getPath());
	if (_request.getHeader("Connection") == "Close")
		_response.setHeader("Connection", "close");
}

bool AMethod::isValid()
{
	if (_request.getParsingFailed())
	{
		_response.setResponseCode(400, "Parsing failed");
		return false;		
	}
	if (!validateURI()
		|| !validateRoute()
		|| !validateHttpVersion()
		|| !validateMethod())
		return false;
	if (_ressource.forbidden())
	{
		_response.setResponseCode(403, "cannot go thru parent");
		return false;
	}
	//CHECK BODY SIZE LIMIT ??
	if (_ressource.isCgi() && executeCgi())
		return false;
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

std::string AMethod::getMimeType()
{
	return _config->getMimeType(_ressource.getExtension());
}

bool	AMethod::executeCgi()
{
	std::string	ext = _ressource.getExtension();
	
	if (!_route.isCgiEnabled(ext.c_str()))
	{
		_response.setResponseCode(501, "Not implemented");
		return EXIT_FAILURE;
	}

	CGI	cgi(_request.getURI().getQuery(),
		_request.getURI().getPath(),
		_request.getMethod(),
		_ressource.getPath().litteral(),
		_request.getHttpVersion(),
		_route.getCgiBinPath(ext.c_str())
	);
	if (cgi.execute())
	{
		_response.setResponseCode(500, "Internal error");
		return EXIT_FAILURE;
	}
	_response.setBody(cgi.getBody());
	for (t_headers::const_iterator it = cgi.getHeaders().begin();
		it != cgi.getHeaders().end(); it++)
		_response.setHeader(it->first, it->second);
	return EXIT_SUCCESS;
}

std::string	AMethod::getInvalidResponse()
{
	return _response.getResponse();
}
