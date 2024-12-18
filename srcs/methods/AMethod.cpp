/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 19:21:14 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/18 11:18:45 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMethod.hpp"

AMethod::AMethod(Config *config, Request & request):
	_config(config),
	_request(request)
{
	_route = _config->getRoute(_request.getURI());
	if (_request.getHeaders().at("Connection") == "close")
		_response.setHeader("Connection", "close");
}

bool AMethod::isValid()
{
	if (!validateURI()
		|| !validateRoute()
		|| !validateHttpVersion()
		|| !validateMethod())
		return false;
	if (_route.isCgi() && executeCgi())
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
	const std::string &	httpVersion = _request.getHttpVersion();

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
	std::string	extension;

	size_t dotPos = path.find_last_of('.');
	if (dotPos != std::string::npos)
		extension = path.substr(dotPos + 1);
	return _config->getMimeType(extension);
}

bool	AMethod::executeCgi()
{
	if (!_route.isCgiEnabled())
	{
		_response.setResponseCode(501, "Not implemented");
		return EXIT_FAILURE;
	}

	CGI	cgi(_request.getURI().getQuery(),
		_request.getURI().getPath(),
		_request.getMethod(),
		_route.getLocalPath(),
		_request.getHttpVersion(),
		_route.getCgiBinPath()
	);
	if (cgi.execute())
		return _response.setResponseCode(500, "Internal error");
	_response.setBody(cgi.getBody());
	for (t_headers::const_iterator it = cgi.getHeaders().begin();
		it != cgi.getHeaders().end(); it++)
		_response.setHeader(it->first, it->second);
	return true;
}