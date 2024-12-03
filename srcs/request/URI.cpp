/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:33:27 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/02 15:43:35 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "URI.hpp"
#include <iostream>

URI::URI()
{
	
}

URI::URI(const char* uri)
{
	(void) uri;
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
