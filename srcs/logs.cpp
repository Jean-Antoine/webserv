/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:28:35 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/11/14 18:14:41 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logs.hpp"

void log(const std::string &message)
{
	std::cout << message << RESET << std::endl;
}

void testLog(const std::string &message)
{
	std::cout << ORANGE << "TEST : " << message << RESET << std::endl;
}

int putError(const std::string &errorMessage)
{
	std::cerr << RED << "ERROR: " << RESET << errorMessage << std::endl;
	return 1;
}
