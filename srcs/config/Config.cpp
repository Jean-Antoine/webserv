/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:27:42 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/12 09:01:45 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <iostream>

Config::Config()
{
}

Config::Config(const JsonData & Data):
	_data(Data)
{
}

Config &	Config::operator=(const Config & src)
{
	this->_data = src._data;
	return *this;
}

Config::~Config()
{
}

int	Config::check()
{
	return 1;
}

const char*	Config::host() const
{
	return _data["host"].string().data();
}

int Config::port() const
{
	return _data["port"].primitive();
}

Route	Config::getRoute(URI & uri)
{
	JsonData 			out;
	const std::string &	uriPath = uri.getPath();
	size_t				score = 0;

	for (int i = 0; i < _data["routes"].size(); i++)
	{
		JsonData &		route = _data["routes"][i];
		std::string &	routePath = route["path"].string();
		size_t			size = routePath.size();
		
		if (size > uriPath.size())
			continue ;
		if (routePath.compare(0, size, uriPath)
			&& size > score
			&& (uriPath[size] == 0
				|| uriPath[size] == '/'))
				{
					score = size;
					out = route;
				}
	}
	Route	route(out, uriPath);
	return route;
}
