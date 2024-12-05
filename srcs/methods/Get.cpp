/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 13:59:05 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/05 14:41:59 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Get.hpp"

Get::Get(Config & config,  Request & request):
	AMethod::AMethod(config, request)
{
}

// int Get::getRessource()
// {
// 	std::ifstream is(_route.getLocalPath().c_str());
// 	if (!is.is_open())
// 		return setResponseCode(404, "Not found");
// 	is >> _response.body;
// 	is.close();
// 	return EXIT_SUCCESS;
// }

int Get::getRessource()
{
	std::ifstream is(_route.getLocalPath().c_str(), std::ios::in | std::ios::binary);
	if (!is.is_open())
		return setResponseCode(404, "Not found");

	is.seekg(0, std::ios::end);
	std::streamsize fileSize = is.tellg();
	is.seekg(0, std::ios::beg);
	if (fileSize <= 0)
		return setResponseCode(204, "No Content");

	std::string content(fileSize, '\0'); // Allouer la chaîne
	if (!is.read(&content[0], fileSize)) // Lire dans la chaîne
	{
		is.close();
		return setResponseCode(500, "Internal Server Error");
	}
	_response.body = content;
	is.close();
	return true;
}

std::string Get::getResponse()
{
	if (!isValid())
		return errorResponse();
	if (!getRessource())
		return errorResponse();
	return buildResponse();
}
