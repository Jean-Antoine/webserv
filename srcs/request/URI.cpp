/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:33:27 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/05 10:22:35 by jeada-si         ###   ########.fr       */
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
	(void) uri;
	(void) _bad;
	// parseHex(uri);
	// std::stringstream	ss(_raw);
	// std::streambuf		*buf = ss.rdbuf();
	// parseScheme(buf);
	// parseHost(buf);
	// parsePath(buf);
	// parseQuery(buf);

	// std::cout << BLUE "Parsing URI: \n"
	// 	<< "host: " << _host << "\n"
	// 	<< "port: " << _port << "\n"
	// 	<< "path: " << _path << "\n"
	// 	<< "query: " << _query << "\n\n" BLUE;
}

URI::~URI()
{
}

// static char	hexToChar(char hex[3])
// {
// 	std::string base = "123456789abcdef";
// 	hex[0] = tolower(hex[0]);
// 	hex[1] = tolower(hex[1]);
	
// 	return (base.find(hex[0]) + 1) * 16 
// 		+ (base.find(hex[1]) + 1);
// }

// void	URI::parseHex(const char* str)
// {
// 	std::stringstream	ss(str);
// 	std::ostringstream	out;
// 	char				hex[3];
// 	int					c;
	
// 	while ((c = ss.get()) != -1)
// 	{
// 		if (c != '%')
// 			out << (char) c;
// 		else
// 		{
// 			ss.get(hex, 3);
// 			out << hexToChar(hex);
// 		}
// 	}
// 	_raw = out.str();
// }

// void	URI::parseScheme(std::streambuf *buf)
// {
// 	char	buffer[9];

// 	memset(buffer, 0, 9);
// 	buf->sgetn(buffer, 7);
// 	if (std::string(buffer) == "http://")
// 	{
// 		_port = "80";
// 		return ;
// 	}
// 	buf->pubseekpos(0);
// 	buf->sgetn(buffer, 8);
// 	if (std::string(buffer) == "https://")
// 	{
// 		_port = "443";
// 		return ;
// 	}
// 	buf->pubseekpos(0);
// }

// bool	isnum(int c)
// {
// 	return c >= '0' && c <= '9';
// }

// bool	URI::parseHost(std::streambuf *buf)
// {
// 	int			count = 0;
// 	std::string	str;

// 	while (std::isalnum(buf->sgetc())
// 		&& buf->sgetc() == '.')
// 	{
// 		if (std::isalnum(buf->sgetc()))
// 		{
// 			if (count > 63)
// 				return EXIT_FAILURE;
// 			str.push_back(buf->sbumpc());
// 			count++;
// 		}
// 		else if (buf->sgetc() == '.')
// 			str.push_back(buf->sbumpc());
// 	}
// 	_host = str;
// 	return EXIT_SUCCESS;
// }

// bool	URI::parsePort(std::streambuf *buf)
// {
// 	if (buf->sgetc() != ':')
// 		return EXIT_SUCCESS;

// 	std::string	out;
// 	while (buf->in_avail() && isnum(buf->sgetc()))
// 		out.push_back(buf->sbumpc());
// 	if (out.size() < 1)
// 		return EXIT_FAILURE;
// 	_port = out;
// 	return EXIT_SUCCESS;
// }

// void	URI::parsePath()
// {
// 	std::stringstream	ss(_raw);
// 	int					c;
// 	std::ostringstream	out;

// 	out << '/';
// 	while ((c = ss.get()) != -1)
// 		if (c == '/')
// 			break;
// 	while ((c = ss.get()) != -1
// 		&& c != '?')
// 		out << (char) c;
// 	_path = out.str();
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
