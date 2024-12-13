/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invalid.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:40:25 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/13 11:09:39 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Invalid.hpp"

Invalid::Invalid(Config *config,  Request & request):
	AMethod::AMethod(config, request)
{
}

std::string Invalid::getResponse()
{
	// if (!isValid()) //toujours invalid
	// 	return errorResponse();
	isValid();
	return _response.getResponse();
}
