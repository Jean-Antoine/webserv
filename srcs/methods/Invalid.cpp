/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invalid.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:40:25 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/03 17:11:01 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Invalid.hpp"

Invalid::Invalid(Config & config,  Request & request):
	AMethod::AMethod(config, request)
{
}

std::string Invalid::getResponse()
{
	// if (!isValid()) //toujours invalid
	// 	return errorResponse();
	isValid();
	return errorResponse();
}
