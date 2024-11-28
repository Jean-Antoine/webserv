/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:28:35 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/11/28 19:41:14 by lpaquatt         ###   ########.fr       */
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

int putError(const std::string &errorMessage, int code) //code = 1 by default
{
	std::cerr << RED << "ERROR: " << RESET << errorMessage << std::endl;
	std::cerr << "Status code: " << code << std::endl;
	return code;
}
