/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invalid.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:40:25 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/11/29 16:29:16 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Invalid.hpp"

std::string Invalid::response()
{
	testLog("wesh");
	std::string out;
	
	out = "HTTP/1.1 400 Bad Request" CRLF
		"Content-Type: text/plain" CRLF
		"Content-Length: 11" CRLF
		"" CRLF
		"Bad Request" CRLF;
	log(out);
	return out;
		
}
