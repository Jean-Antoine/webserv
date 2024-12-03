/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 19:21:14 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/02 15:44:50 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMethod.hpp"

AMethod::AMethod(Config & config, Request & request):
	_config(config),
	_method(request.getMethod()),
	_uri(request.getURI()),
	_httpVersion(request.getHttpVersion()),
	_headers(request.getHeaders()),
	_body(request.getBody())
{
}

int AMethod::validateMethod() // + verifier selon la config
{
	if (_method == INVALID)
		return 501; //setResponseCode(501, "Method not implemented"); //a gerer
	return 0;
}

// static bool isValidHost(std::string host)
// {
// 	// if (host.empty() || host.find("..") != std::string::npos || !std::isalnum(host[0])
// 	// 	|| (host.rfind('.') != std::string::npos &&  host.rfind('.') >= host.length() - 2))
// 	// 	return false;
// 	// for (size_t i = 0; i < host.length(); ++i)
// 	// {
// 	// 	char c = host[i];
// 	// 	if (!std::isalnum(c) && c != '.' && c != '-')
// 	// 		return false;
// 	// }
// 	return true;
// }


//TBD WITH CLASS URI
//accepts absolute path and http(s)://host/path 
//atttion > pas possible de mettre un port..?
int AMethod::validateReqURI()
{
	// std::string	reqURI = _reqLine.reqURI;
	// if (reqURI.empty())
	// 	return 400; //setResponseCode(400, "Bad Request (URI is empty)");
	// for (size_t i = 0; i < reqURI.size(); ++i) {
	// 	char c = reqURI[i];
	// 	if (!(std::isprint(c) && c != ' ' && c != '\t'))
	// 		return 400; //setResponseCode(400, "Bad Request (URI contains invalid characters)");
	// }
	// if (reqURI[0] != '/'
	// 	&& ((reqURI.compare(0, 7, "http://") != 0 && reqURI.compare(0, 8, "https://") != 0)
	// 		|| !isValidHost(reqURI.substr(reqURI.find("://") + 3))))
	// 		return 400; //setResponseCode(400, "Bad Request (Invalid URI)");
	return true;
}

int AMethod::validateHttpVersion()
{
	if( _httpVersion.length() != 8
		|| _httpVersion.compare(0, 5, "HTTP/") != 0
		|| !std::isdigit(_httpVersion[5])
		|| _httpVersion[6] != '.'
		|| !std::isdigit(_httpVersion[7]))
		return 400; // setResponseCode(400, "Bad request (Invalid Http Version format)");
	if (_httpVersion != "HTTP/1.1")
		return 505; // setResponseCode(505, "HTTP Version Not Supported");
	return 0;
}
