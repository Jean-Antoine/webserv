/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logs.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 12:59:02 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/13 08:32:48 by jeada-si         ###   ########.fr       */
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
	printTab(true);
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

void	Logs::printTab(int reset) const
{
	static int i = 0;

	if (reset)
		i = -1;
	if (i++ == 0)
		std::cout << "\t";
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
	if (!VERBOSE)
		return logs;
	logs.printTab(false);
	std::cout << logs.getColor(false);
	for (std::string::const_iterator it = str.begin();
	it != str.end(); it++)
	{
		std::cout << (char) *it;
		if (*it == '\n' && it != str.end() - 1)
			std::cout << (char) '\t';
	}
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
	if (!VERBOSE)
		return logs;
	logs.printTab(false);
	std::cout << logs.getColor(false) << i;
	return logs;
}
