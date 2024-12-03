/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:17:48 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/03 17:11:13 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Get.hpp"

Get::Get(Config & config,  Request & request):
	AMethod::AMethod(config, request)
{
}

std::string Get::getResponse()
{
	if (!isValid())
		return errorResponse();
	return buildResponse();
}
