/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:36:15 by lpaquatt          #+#    #+#             */
/*   Updated: 2025/01/08 17:53:53 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Post.hpp"

Post::Post(Config *config,  Request & request):
	AMethod::AMethod(config, request)
{
}

int Post::parseFormUrlEncoded(std::map<std::string, std::string> &parsedData)
{
	std::istringstream stream(_request.getBody());
	std::string pair;

	while (std::getline(stream, pair, '&'))
	{
		size_t pos = pair.find('=');
		if (pos != std::string::npos)
		{
			std::string key = pair.substr(0, pos);
			std::string value = pair.substr(pos + 1);
			parsedData[key] = value;
		}
		else
		{
			_response.setResponseCode(400, " wrong format of content\nContent: " + _request.getBody()); // todo @leon: a checker
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

void Post::postFormUrlEncoded()
{
	std::map<std::string, std::string> parsedData;
	if (parseFormUrlEncoded(parsedData))
		return;
}

// other "common" types (not in mandatory subject) text/plain application/xml application/octet-stream application/graphql
// cf https://lukewang.dev/posts/2024/06/27/common-post-body-types-in-http/
void Post::postContent()
{
	t_headers headers = _request.getHeaders(); // @leon voir si peux faire + propre mais getHeaders renvoie une const donc ne peut pas utiliser [] si fais pas copie
	std::string contentType = headers["Content-Type"];
	if (contentType.empty()){
		_response.setResponseCode(400, "content type is missing");
		return;
	}
	if (contentType == "application/x-www-form-urlencoded")
		postFormUrlEncoded();
	// else if (contentType.find("multipart/form-data") != std::string::npos)
	// 	return postMultipartFormData();
	// else if (contentType == "application/json")
	// 	return postJson();
	else
		_response.setResponseCode(415, contentType);
}


std::string Post::getResponse()
{
	if (_ressource.isCgi())
		return _response.getResponse();
	// std::string path = _route.getLocalPath();
	postContent();
	return _response.getResponse();
}
