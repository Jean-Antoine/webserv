/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:27:42 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/12 10:20:26 by jeada-si         ###   ########.fr       */
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
		if (routePath.compare(0, size, uriPath) == 0
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

void Config::parseMimeTypes(std::string mimeFilePath)
{
	std::ifstream mimeFile(mimeFilePath.c_str());
	if (!mimeFile.is_open())
		return; //todo: a tester ..?

	std::string line;
	while (std::getline(mimeFile, line))
	{
		if (line.empty() || line[0] == '#') continue;

		std::istringstream iss(line);
		std::string mimeType, extension;
		if (iss >> mimeType) {
			while (iss >> extension) {
				_mimeTypes[extension] = mimeType;
			}
		}
	}
}

std::string Config::getMimeType(const std::string& extension)
{
	if (_mimeTypes.find(extension) != _mimeTypes.end())
		return _mimeTypes[extension];
	return "application/octet-stream"; // default value
}
