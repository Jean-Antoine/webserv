/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logs.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 12:59:02 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/19 14:09:10 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logs.hpp"
#include <iostream>

Logs::Logs(char const* color):
	_color(color)
{
}

Logs::~Logs()
{
	printDate(true);
	std::cout << RESET;
}

void	Logs::printDate(int reset) const
{
	static int i = 0;
	
	if (reset)
		i = -1;
	if (i++ == 0)
		std::cout << getColor(true) << "[" << getDate() << "] ";
}

const std::string	Logs::getColor(bool bold) const
{
	if (bold)
		return BOLD + _color;
	return SIMPLE + _color;
}

const Logs&	operator<<(const Logs& logs, const std::string & str)
{
	logs.printDate(false);
	std::cout << logs.getColor(true) << str;
	return logs;
}

const Logs&	operator<(const Logs& logs, const std::string & str)
{
	if (VERBOSE)
		std::cout << logs.getColor(false) << str;
 	return logs;
}

const Logs&	operator<<(const Logs& logs, int i)
{
	logs.printDate(false);
	std::cout << logs.getColor(true) << i;
	return logs;
}

const Logs&	operator<(const Logs& logs, int i)
{
	if (VERBOSE)
		std::cout << logs.getColor(false) << i;
	return logs;
}
