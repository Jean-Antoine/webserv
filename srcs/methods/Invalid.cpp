/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invalid.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:40:25 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/17 22:05:00 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Invalid.hpp"

Invalid::Invalid(Config *config,  Request & request):
	AMethod::AMethod(config, request)
{
}

std::string Invalid::getResponse()
{
	if (!_request.getParsingFailed())
		validateMethod();
	else
		_response.setResponseCode(400, "Bad Request", false); 
	return _response.getResponse();
}
