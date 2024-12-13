/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 19:21:14 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/13 13:28:58 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMethod.hpp"

AMethod::AMethod(Config *config, Request & request):
	_config(config),
	_request(request)
{
	_route = _config->getRoute(request.getURI());
	if (_request.getHeaders()["Connection"] == "close")
		_response.setHeader("Connection", "close");
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
	_response.setResponseCode(405, "Method Not Allowed");
	_response.setHeader("Allow", concatStrVec(_route.getAllowedMethods(), ", ", true));
	return false;	
}

bool	AMethod::validateMethod()
{
	if (_request.getMethod() == "INVALID")
		return _response.setResponseCode(501, "Method not implemented");
	return checkAllowedMethods();
}

bool	AMethod::validateURI()
{
	if (_request.getURI().bad())
		return _response.setResponseCode(400, "Bad URI");
	return true;
}

bool	AMethod::validateRoute()
{
	if (_route.bad())
		return _response.setResponseCode(404, "Not found");
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
		return _response.setResponseCode(400, "Bad request (Invalid Http Version format)");
	if (httpVersion != "HTTP/1.1")
	{
		_response.setHeader("Connection", "close");
		return _response.setResponseCode(505, "HTTP Version Not Supported");
	}
	return true;
}

std::string AMethod::getMimeType(const std::string & path)
{
	return _config->getMimeType(getExtension(path));
}
