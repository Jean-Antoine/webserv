/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:41:07 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/06 10:21:06 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"
#include <iostream>

static std::string	concatPath(const std::string & prefix, const std::string & path)
{
	std::string	out = prefix;

	if (*out.rbegin() != '/')
		out.append("/");
	if (path[0] == '/')
		out.append(path.begin() + 1, path.end());
	else
		out.append(path);
	return out;
}

Route::Route():
	_routePath(empty::string)
{
}

// todo: @JA faudrait pas verifier les paths genre bloquer les .. pour pas sortir du dossier root
Route::Route(JsonData & data, t_str uriPath):
	_routePath(data["path"].string()),
	_data(data)
{
	if (_data.empty())
		return ;

	_relativePath = uriPath.substr(_routePath.size(), std::string::npos);
	_localPath = concatPath(getRoot(), _relativePath);
	_default = concatPath(getRoot(), _data["default_file"].string());
	Logs(MAGENTA) < "Local path is " < _localPath < "\n";
}

Route::Route(const Route &src):
	_routePath(src._routePath),
	_data(src._data)
{
	*this = src;
}

Route& Route::operator=(const Route &src)
{
	_routePath = src._routePath;
	_data = src._data;
	_relativePath = src._relativePath;
	_localPath = src._localPath;
	_default = src._default;
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
	return _default;
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
	if (getExtension() == "php" || getExtension() == "py")
		return true;
	return false;
}

bool	Route::isCgiEnabled() const
{	
	return !_data["cgi"][getExtension().c_str()].empty();
}

const std::string &	Route::getCgiBinPath() const
{
	return _data["cgi"][getExtension().c_str()].string();
}

std::string	Route::getExtension() const
{
	std::string	ext;

	if (_localPath.find_last_of('.') == std::string::npos)
		return ext;
	ext = _localPath.substr(_localPath.find_last_of('.') + 1);
	return ext;
}
