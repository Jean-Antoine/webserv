/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:27:42 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/22 14:56:16 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Logs.hpp"
#include "Route.hpp"
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
	if (_data["port"].empty() || _data["port"].type() != PRIMITIVE)
		return EXIT_FAILURE;
	if (_data["host"].empty() || _data["host"].type() != STRING)
		return EXIT_FAILURE;
	if (_data["routes"].empty() || _data["routes"].type() != OBJECTARRAY)
		return EXIT_FAILURE;
	if (!_data["server_names"].empty() && _data["server_names"].type() != STRINGARRAY)
	// if (_data["server_names"].type() != STRINGARRAY)
		return EXIT_FAILURE;
	if (!_data["client_max_body_size"].empty() && _data["client_max_body_size"].type() != PRIMITIVE)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

const char*	Config::host() const
{
	return _data["host"].string().data();
}

int Config::port() const
{
	return _data["port"].primitive();
}

const t_strArray&	Config::getServerNames() const
{
	return _data["server_names"].stringArray();
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
	Logs(PURPLE) < "Route: "
		< route.getPath() < " = "
		< route.getRoot() < "\n";
	return route;
}

void Config::parseMimeTypes(std::string mimeFilePath)
{
	// std::ifstream	mimeFile("bad");
	// (void) mimeFilePath;
	std::ifstream	mimeFile(mimeFilePath.c_str());
	if (!mimeFile.is_open())
		return;

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
	return "text/plain"; // default value @leontinepaq reverifier default valude de nginx
}

std::string	Config::getErrorPage(int code, bool forceDefault) const
{
	std::string &custom = _data["default_error_pages"][to_string(code).c_str()].string();
	if (forceDefault || custom.empty())
		return ERROR_DIR + to_string(code) + ".html";
	return custom;
}

long	Config::getMaxBodySize() const
{
	if (_data["client_max_body_size"].empty())
		return CLIENT_MAX_BODY_SIZE;
	return _data["client_max_body_size"].primitive();
}
