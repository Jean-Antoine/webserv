/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logs.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 12:59:02 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/18 15:45:24 by jeada-si         ###   ########.fr       */
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
	
	if (i++ == 0)
		std::cout << _color << "[" << getDate() << "] ";
	if (reset)
		i = 0;
}

const std::string &	Logs::getColor() const
{
	return _color;
}

const Logs&	operator<<(const Logs& logs, const std::string & str)
{
	logs.printDate(false);
	std::cout << logs.getColor() << str;
	return logs;
}

const Logs&	operator<(const Logs& logs, const std::string & str)
{
	if (!VERBOSE)
		return logs;
	std::cout << RESET << str;
	return logs;
}

const Logs&	operator<<(const Logs& logs, int i)
{
	logs.printDate(false);
	std::cout << logs.getColor() << i;
	return logs;
}

const Logs&	operator<(const Logs& logs, int i)
{
	if (VERBOSE)
		std::cout << RESET << i;
	return logs;
}
