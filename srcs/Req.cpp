/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 08:09:43 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/28 14:51:06 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Req.hpp"
#include <iostream>

Req::Req(const char *req):
	_data(req)
{	
	_badFormat = 0;
	_lines = split(_data, CRLF);
	purgeCRLF();
	parseReqLine();
	parseHeaders();
	// parseBody();
}

Req::~Req()
{
}

void	Req::purgeCRLF()
{
	t_str_vec::iterator	it = _lines.begin();

	while (it != _lines.end() && it->size() == 0)
		_lines.erase(it);
}

void	Req::parseReqLine()
{
	_badFormat += _lines.size() < 1;
	if (_badFormat)
		return ;
	
	t_str_vec	reqLine = split(_lines[0], " ");
	_badFormat += reqLine.size() != 3;
	if (_badFormat)
		return ;
	_method = reqLine[0];
	_URI = reqLine[1];
	_version = reqLine[2];
}

void	Req::parseHeaders()
{
	_badFormat += _lines.size() < 2;
	if (_badFormat)
		return ;
	
	for (t_str_vec::iterator it = _lines.begin() + 1;
	it != _lines.end(); it++)
	{
		if (it->size() == 0)
			break ;
		size_t		pos = it->find(": ");
		_badFormat += pos == std::string::npos;
		if (_badFormat)
			return ;
		std::string	key = it->substr(0, pos);
		_badFormat = pos + 2 > it->size();
		if (_badFormat)
			return ;
		std::string	value = it->substr(pos + 2, std::string::npos);
		rmLeadSpaces(value);
		_badFormat += key.size() == 0 || value.size() == 0;
		if (_badFormat)
			return ;
		if (_headers[key].size() == 0)
			_headers[key] = value;
		else
			_headers[key].append(",").append(value);
	}
}

int	Req::bad()
{
	return _badFormat;
}

// void	Req::parseBody()
// {
// 	t_str_vec::iterator	it = _lines.begin();
	
// 	while (it != _lines.end())
// 		if (it++->size() == 0)
// 			break;
// 	if (it != _lines.end())
		
// }

std::string Req::response()
{
	std::string	out = "HTTP/1.1 200 OK" CRLF CRLF;
	
	out.append("<h1> BUILDING RESPONSE </h1>");
	out.append(CRLF CRLF);
	return out;
}