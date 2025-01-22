/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:41:07 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/22 12:38:12 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"
#include <iostream>

Route::Route()
{
	_data = &empty::data;
}

Route::Route(JsonData *data): _data(data)
{
}

Route::Route(const Route &src)
{
	*this = src;
}

Route& Route::operator=(const Route &src)
{
	_data = src._data;
	return *this;
}

Route::~Route()
{
}

const t_strVec &	Route::getAllowedMethods() const
{
	return (*_data)["methods"].stringArray();
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
	if ((*_data)["methods"].empty())
		return false;

	const t_strVec &	methods = getAllowedMethods();
	return isIn(method, methods);
}

const std::string &	Route::getPath() const
{
	return (*_data)["path"].string();
}

const std::string & Route::getRoot() const
{
	return (*_data)["root"].string();
}

const std::string & Route::getDefaultFile() const
{
	return (*_data)["default_file"].string();
}

bool	Route::isDirListEnabled() const
{
	if ((*_data)["directory_listing"].empty())
		return false;
	return (*_data)["directory_listing"].primitive();
}

bool	Route::isCgiEnabled(const char *ext) const
{	
	return !(*_data)["cgi"][ext].empty();
}

const std::string &	Route::getCgiBinPath(const char *ext) const
{
	return (*_data)["cgi"][ext].string();
}

bool	Route::empty() const
{
	return _data == NULL || _data->empty();
}

bool	Route::isRedirectionEnabled() const
{
	return !(*_data)["redirection"]["enabled"].empty()
		&& (*_data)["redirection"]["enabled"].primitive() == true;
}

const std::string &	Route::getRedirection() const
{
	return (*_data)["redirection"]["url"].string();
}

bool	Route::isUploadsEnabled() const
{
	return !(*_data)["uploads"]["enabled"].empty()
		&& (*_data)["uploads"]["enabled"].primitive() == true;
}

const std::string &	Route::getUploads() const
{
	return (*_data)["uploads"]["directory"].string();
}
