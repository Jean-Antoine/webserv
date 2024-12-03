/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:33:27 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/03 15:59:54 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "URI.hpp"
#include <iostream>

URI::URI()
{
	
}

static char	hexToChar(char	hex[3])
{
	hex[0] = tolower(hex[0]);
	hex[1] = tolower(hex[1]);
	std::string base = "123456789abcdef";
	
	return (base.find(hex[0]) + 1) * 16 + base.find(hex[1]) + 1;
}

URI::URI(const char* uri)
{
	std::stringstream	ss(uri);
	std::ostringstream	out;
	char				hex[3];
	int					c = ss.get();
	
	while (c != -1)
	{
		if (c != '%')
			out << (char) c;
		else
		{
			ss.get(hex, 3);
			out << hexToChar(hex);
		}
		c = ss.get();
	}
	_raw = out.str();
}

URI::URI(const URI &src)
{
	*this = src;
}

URI& URI::operator=(const URI &src)
{
	_host = src._host;
	_port = src._port;
	_path = src._path;
	_query = src._query;
	return *this;
}

URI::~URI()
{
	//std::cout << BLUE "[URI] Destructor called" RESET << std::endl;
}
