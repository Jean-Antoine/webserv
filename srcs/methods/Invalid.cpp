/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invalid.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:40:25 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/18 17:41:14 by lpaquatt         ###   ########.fr       */
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
		_response.setResponseCode(400, "", false); 
	return _response.getResponse();
}
