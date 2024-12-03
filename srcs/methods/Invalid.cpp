/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invalid.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:40:25 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/03 09:00:21 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Invalid.hpp"

Invalid::Invalid(Config & config,  Request & request):
	AMethod::AMethod(config, request)
	{
		
	}


std::string Invalid::response()
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
