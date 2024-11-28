/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 19:21:14 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/11/28 19:28:31 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMethod.hpp"

int AMethod::validateMethod() // + verifier selon la config
{
	if (_reqLine.method == INVALID)
		return 501; //setResponseCode(501, "Method not implemented"); //a gerer
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
