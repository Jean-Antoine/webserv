/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:27:42 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/05 15:38:11 by jeada-si         ###   ########.fr       */
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
	(void) uri;
	return _data["routes"][0];
}
