/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:27:42 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/08 16:00:55 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <iostream>

Config::Config()
{
	parseMimeTypes(MIME_TYPES_FILE);
}

Config::Config(const JsonData & Data):
	_data(Data)
{
	parseMimeTypes(MIME_TYPES_FILE);
}

Config &	Config::operator=(const Config & src)
{
	this->_data = src._data;
	this->_mimeTypes = src._mimeTypes;
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

Route	Config::getRoute(const URI & uri)
{
	Route 	route;
	Path	uriPath(uri.getPath());
	size_t	score = 0;

	for (int i = 0; i < _data["routes"].size(); i++)
	{
		Route	RouteIdx(&(_data["routes"][i]));
		Path	routePath = RouteIdx.getPath();

		if (routePath.in(uriPath)
			&& routePath.compare(uriPath) >= score)
		{
			score = routePath.compare(uriPath);
			route = RouteIdx;
		}
	}
	Logs(RED) < "Route: "
		< route.getPath() < " = "
		< route.getRoot() < "\n";
	return route;
}

void Config::parseMimeTypes(std::string mimeFilePath)
{
	std::ifstream	mimeFile(mimeFilePath.c_str());
	if (!mimeFile.is_open())
		return; //todo: a tester ..?

	std::string	line;
	while (std::getline(mimeFile, line))
	{
		if (line.empty() || line[0] == '#') continue;

		std::istringstream	iss(line);
		std::string			mimeType, extension;
		if (iss >> mimeType) {
			while (iss >> extension) {
				_mimeTypes[extension] = mimeType;
			}
		}
	}
}

std::string Config::getMimeType(const std::string extension)
{
	if (!extension.empty()
		&& _mimeTypes.find(extension) != _mimeTypes.end())
		return _mimeTypes[extension];
	return "application/octet-stream"; // default value
}
