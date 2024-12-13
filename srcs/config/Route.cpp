/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:41:07 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/13 16:58:36 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"
#include <iostream>

Route::Route()
{
}

Route::Route(JsonData & data, t_str uriPath): _data(data)
{
	std::cout << MAGENTA "Building route to local path: \n";
	if (_data.empty())
		return ;

	std::string	routePath = _data["path"].string();

	_localPath = concatPath(getRoot(), uriPath.substr(routePath.size(), std::string::npos));
	std::cout << _localPath << RESET "\n\n";
}

Route::Route(const Route &src): _data(src._data)
{
	*this = src;
}

Route& Route::operator=(const Route &src)
{
	_data = src._data;
	_localPath = src._localPath;
	return *this;
}

Route::~Route()
{
}

bool	Route::bad()
{
	return _data.empty();
}

const t_strVec &	Route::getAllowedMethods() const
{
	return _data["methods"].stringArray();
}

static bool	isIn(const std::string & str, const t_strVec & vec)
{
	for (t_strVec::const_iterator it = vec.begin();
		it != vec.end(); it++)
		if (it->compare(str) == 0)
			return true;
	return false;
}

bool	Route::isMethodAllowed(std::string method) const
{
	if (_data["methods"].empty())
		return false;

	const t_strVec &	methods = getAllowedMethods();
	return isIn(method, methods);
}

const std::string	Route::getDefaultFile() const
{
	if (_data["default_file"].empty())
		return "index.html";
	return _data["default_file"].string();
}

const std::string &	Route::getLocalPath() const
{
	return _localPath;
}

const std::string & Route::getRoot() const
{
	return _data["root"].string();
}

bool	Route::isDirListEnabled() const
{
	if (_data["directory_listing"].empty())
		return false;
	return _data["directory_listing"].primitive();
}

bool	Route::isCgi() const
{
	std::string extension = getExtension(_localPath);

	if (extension == "php" || extension == "py")
		return true;
	return false;
}

bool	Route::isCgiEnabled() const
{
	std::string	extension = getExtension(_localPath);
	
	return !_data["cgi"][extension.c_str()].empty();
}

const std::string &	Route::getCgiBinPath() const
{
	std::string	extension = getExtension(_localPath);
	
	return _data["cgi"][extension.c_str()].string();
}

bool Route::isCGIEnabled() const // todo: a checker @JA
{
	if (_data["cgi"].empty() || _data["cgi"]["enabled"].empty())
		return false;
	return _data["cgi"]["enabled"].primitive();
}

const t_strVec &	Route::getCGIExtensions() const
{
	return _data["cgi"]["extensions"].stringArray();
}

bool Route::isCGIExtension(std::string extension)
{
	if (_data["cgi"].empty() || _data["cgi"]["extensions"].empty())
		return false;

	const t_strVec &	extensions = getCGIExtensions();
	for (t_strVec::const_iterator it = extensions.begin();
		it != extensions.end(); it++)
		if (it->compare(extension) == 0)
			return true;
	return false;

}
