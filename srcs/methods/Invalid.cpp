/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invalid.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:40:25 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/02 19:24:44 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Invalid.hpp"

std::string Invalid::getResponse()
{
	if (!isValid())
		return errorResponse();
	std::string out;
	
	out = "HTTP/1.1 200 OK" CRLF
		"Content-Type: text/plain" CRLF
		"Content-Length: 11" CRLF
		"" CRLF
		"Test : good Request" CRLF;
	return out;
}
