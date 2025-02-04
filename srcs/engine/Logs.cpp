/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logs.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 12:59:02 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/28 08:50:24 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logs.hpp"
#include "Client.hpp"
#include <iostream>

Logs::Logs(char const* color):
	_color(color),
	_out(std::cout)
{
}

Logs::Logs(char const* color, bool error):
	_color(color),
	_out(error ? std::cerr : std::cout)
{
}

Logs::~Logs()
{
	printDate(true);
	printTab(true);
	_out << RESET;
}

void	Logs::printDate(int reset) const
{
	static int i = 0;
	
	if (reset)
		i = -1;
	if (i++ == 0)
		_out << BOLD CYAN << "[" << getDate() << "] ";
}

void	Logs::printTab(int reset) const
{
	static int i = 0;

	if (reset)
		i = -1;
	if (i++ == 0)
		_out << TAB;
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
	logs._out << logs.getColor(true) << str;
	return logs;
}

const Logs&	operator<(const Logs& logs, const std::string & str)
{
	if (!VERBOSE)
		return logs;
	logs.printTab(false);
	logs._out << logs.getColor(false);
	for (std::string::const_iterator it = str.begin();
	it != str.end(); it++)
	{
		logs._out << (char) *it;
		if (*it == '\n' && it != str.end() - 1)
			logs._out << TAB;
	}
	return logs;
}

const Logs&	operator<<(const Logs& logs, int i)
{
	logs.printDate(false);
	logs._out << logs.getColor(true) << i;
	return logs;
}

const Logs&	operator<(const Logs& logs, int i)
{
	if (!VERBOSE)
		return logs;
	logs.printTab(false);
	logs._out << logs.getColor(false) << i;
	return logs;
}

int error(const char *prefix)
{	
	Logs(RED, true) << "[ERROR] " << prefix << ": "
		<< strerror(errno) << "\n";
	return EXIT_FAILURE;
}

const Logs&	operator<<(const Logs& logs, const Client & clt)
{
	logs.printDate(false);
	logs << clt.getHost() << ":" << clt.getService();
	// if (!clt.getSessionId().empty())
	// 	logs << "{" << clt.getSessionId() << "}";
	return logs;
}

const Logs &	operator<(const Logs & logs, const Message & msg)
{
	if (!VERBOSE)
		return logs;
	for (t_headers::const_iterator it = msg.getHeaders().begin();
	it != msg.getHeaders().end(); it++)
	{
		logs << TAB;
		logs._out << logs.getColor(true) << it->first << ": ";
		logs._out << logs.getColor(false) << it->second << "\n";
	}
	return logs;
}
