/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:41:07 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/12 09:25:15 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"
#include <iostream>

Route::Route()
{
}

Route::Route(JsonData & data, t_str uriPath): _data(data)
{
	if (_data.empty())
		return ;

	std::string	routePath = _data["path"].string();

	_localPath = getRoot() + "/";
	_localPath.append(uriPath, routePath.size(), std::string::npos);
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

bool	Route::isMethodAllowed(std::string method) const
{
	if (_data["methods"].empty())
		return false;

	const t_strVec &	methods = getAllowedMethods();
	for (t_strVec::const_iterator it = methods.begin();
		it != methods.end(); it++)
		if (it->compare(method) == 0)
			return true;
	return false;
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
