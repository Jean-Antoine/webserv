/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:33:27 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/02 13:33:33 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "URI.hpp"
#include <iostream>

URI::URI()
{
	//std::cout << BLUE "[URI] Default constructor called" RESET << std::endl;
}

URI::URI(const URI &src)
{
	//std::cout << BLUE "[URI] Copy constructor called" RESET << std::endl;
}

URI& URI::operator=(const URI &src)
{
	//std::cout << BLUE "[URI] Assignment operator called" RESET << std::endl;
}

URI::~URI()
{
	//std::cout << BLUE "[URI] Destructor called" RESET << std::endl;
}
