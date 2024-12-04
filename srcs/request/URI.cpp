/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:33:27 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/04 10:48:37 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "URI.hpp"
#include <iostream>

URI::URI():
	_raw(""),
	_host(""),
	_port("80"),
	_path(""),
	_query(""),
	_bad(false)
{
	
}

URI::URI(const char* uri):
	_raw(""),
	_host(""),
	_port("80"),
	_path(""),
	_query(""),
	_bad(false)
{
	parseHex(uri);
	parseHost();
	parsePath();
	parseQuery();

	std::cout << BLUE "Parsing URI: \n"
		<< "_raw " << _raw << "\n"
		<< "_host " << _host << "\n"
		<< "_port " << _port << "\n"
		<< "_path " << _path << "\n"
		<< "_query " << _query << "\n" BLUE;
}

URI::~URI()
{
}

static char	hexToChar(char hex[3])
{
	std::string base = "123456789abcdef";
	hex[0] = tolower(hex[0]);
	hex[1] = tolower(hex[1]);
	
	return (base.find(hex[0]) + 1) * 16 
		+ (base.find(hex[1]) + 1);
}

void	URI::parseHex(const char* str)
{
	std::stringstream	ss(str);
	std::ostringstream	out;
	char				hex[3];
	int					c;
	
	while ((c = ss.get()) != -1)
	{
		if (c != '%')
			out << (char) c;
		else
		{
			ss.get(hex, 3);
			out << hexToChar(hex);
		}
	}
	_raw = out.str();
}

void	URI::parseHost()
{
	if (_raw[0] == '/')
		return ;
		
	std::stringstream	ss(_raw);
	std::ostringstream	out;
	int					c;
	
	if (_raw.compare(0, 7, "http://") == 0)
		_raw.erase(0, 7);
	if (_raw.compare(0, 8, "https://") == 0)
		_raw.erase(0, 8);
	while ((c = ss.get()) != -1
		&& (c == '/' || c == ':'))
		out << (char) c;
	_host = out.str();
}

void	URI::parsePort()
{
	if (_raw.find(':') == std::string::npos)
		return ;
	
	std::stringstream	ss(_raw);
	int					c;
	std::ostringstream	out;
	while ((c = ss.get()) != -1)
		if (c == ':')
			break;
	while ((c = ss.get()) != -1
		&& c >= '0' && c <= '9')
		out << (char) c;
	_port = out.str();
}

void	URI::parsePath()
{
	std::stringstream	ss(_raw);
	int					c;
	std::ostringstream	out;

	out << '/';
	while ((c = ss.get()) != -1)
		if (c == '/')
			break;
	while ((c = ss.get()) != -1
		&& c != '?')
		out << (char) c;
	_path = out.str();
}

void	URI::parseQuery()
{
	if (_raw.find('?') == std::string::npos)
		return ;

	std::stringstream	ss(_raw);
	int					c;
	std::ostringstream	out;

	while ((c = ss.get()) != -1)
		if (c == '?')
			break;
	while ((c = ss.get()) != -1)
		out << (char) c;
	_query = out.str();
}

int	URI::bad() const
{
	return _bad;
}
