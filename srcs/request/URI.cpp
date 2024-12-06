/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:33:27 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/06 11:53:06 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "URI.hpp"
#include <iostream>

URI::URI():
	_raw(""),
	_host(""),
	_port("80"),
	_path("/"),
	_query(""),
	_bad(false)
{
}

URI::URI(const char* uri):
	_raw(""),
	_host(""),
	_port("80"),
	_path(""),
	_query("/"),
	_bad(false)
{
	(void) uri;
	(void) _bad;
	parseHex(uri);
	std::stringstream	ss(_raw);
	_buf = ss.rdbuf();
	parseScheme();
	_bad = parseHost()
		|| parsePort();

	std::cout << BLUE "Parsing URI: \n"
		<< "bad: " << _bad << "\n"
		<< "host: " << _host << "\n"
		<< "port: " << _port << "\n"
		<< "path: " << _path << "\n"
		<< "query: " << _query << "\n\n" BLUE;
}

URI::~URI()
{
}

int	URI::next()
{
	return _buf->sgetc();
}

int	URI::bump()
{
	return _buf->sbumpc();
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

void	URI::parseScheme()
{
	char	buffer[9];

	memset(buffer, 0, 9);
	_buf->sgetn(buffer, 7);
	if (std::string(buffer) == "http://")
	{
		_port = "80";
		return ;
	}
	_buf->pubseekpos(0);
	_buf->sgetn(buffer, 8);
	if (std::string(buffer) == "https://")
	{
		_port = "443";
		return ;
	}
	_buf->pubseekpos(0);
}

bool	isnum(int c)
{
	return c >= '0' && c <= '9';
}

static bool checkDomain(std::string & str)
{
	if (str[0] == '-')
		return false;

	t_strVec	segments = split(str, ".");
	if (segments.size() < 2)
		return false;
	for (t_strVec::iterator it = segments.begin();
		it != segments.end() - 1; it++)
	{
		if (it->size() > 63)
			return false;
		for (std::string::iterator itt = it->begin();
		itt != it->end(); itt++)
			if (!std::isalnum(*itt) && *itt != '-')
				return false;
	}
	
	std::string	& topLevel = segments.back();
	if (topLevel.size() < 2
		|| topLevel.size() > 6)
		return false;
	for (std::string::iterator it = topLevel.begin();
		it != topLevel.end(); it++)
		if (isnum(*it))
			return false;
	return true;
}

static bool	checkIPV4(std::string & str)
{
	t_strVec	segments = split(str, ".");
	if (segments.size() != 4)
		return false;
	for (t_strVec::iterator it = segments.begin();
		it != segments.end(); it++)
	{
		if (it->size() > 3 || it->size() == 0)
			return false;
		if (std::strtod(it->c_str(), NULL) > 256)
			return false;
		for (std::string::iterator itt = it->begin();
		itt != it->end(); itt++)
			if (!isnum(*itt))
				return false;
	}
	return true;
}

static bool	isHexChar(int c)
{
	if (isnum(c))
		return true;
	if (c >= 'a' && c <= 'f')
		return true;
	if (c >= 'A' && c <= 'F')
		return true;
	return false;
}

static bool	checkIPV6(std::string & str)
{
	t_strVec	segments = split(str, ":");
	if (segments.size() < 3 || segments.size() > 8)
		return false;
	for (t_strVec::iterator it = segments.begin();
		it != segments.end(); it++)
	{
		if (it->size() > 4)
			return false;
		for (std::string::iterator itt = it->begin();
		itt != it->end(); itt++)
			if (!isHexChar(*itt))
				return false;
	}
	return true;
}

static bool	checkHost(std::string & str)
{
	if (str.compare("localhost") == 0
		|| checkDomain(str)
		|| checkIPV4(str)
		|| checkIPV6(str))
		return true;
	return false;
}

bool	URI::parseHost()
{
	if (next() == '/')
		return EXIT_SUCCESS;
	std::string	str;
	int			brackets = next() == '[';

	if (brackets)
		bump();
	while (std::isalnum(next())
		|| next() == '.'
		|| (brackets && next() == ':'))
		str.push_back(bump());
	std::cout << MAGENTA << str << "\n" RESET;
	if (brackets && next() != ']')
		return EXIT_FAILURE;
	if (brackets && next() == ']')
		bump();
	if (!checkHost(str))
		return EXIT_FAILURE;
	_host = str;
	return EXIT_SUCCESS;
}

bool	URI::parsePort()
{
	if (next() != ':')
		return EXIT_SUCCESS;

	bump();
	std::string	str;
	while (_buf->in_avail() && isnum(next()))
		str.push_back(bump());
	if (str.size() == 0)
		return EXIT_FAILURE;
	_port = str;
	return EXIT_SUCCESS;
}

// bool	URI::parsePath()
// {
// 	if (_buf->in_avail() == 0
// 		|| next() == '?'
// 		|| next() == '#')
// 		return EXIT_SUCCESS;
// 	if (next() != '/')
// 		return EXIT_FAILURE;
// 	while (next() == '/' || )
// }

// void	URI::parseQuery()
// {
// 	if (_raw.find('?') == std::string::npos)
// 		return ;

// 	std::stringstream	ss(_raw);
// 	int					c;
// 	std::ostringstream	out;

// 	while ((c = ss.get()) != -1)
// 		if (c == '?')
// 			break;
// 	while ((c = ss.get()) != -1)
// 		out << (char) c;
// 	_query = out.str();
// }

// int	URI::bad() const
// {
// 	return _bad;
// }
