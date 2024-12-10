/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:41:07 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/10 14:48:56 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"
#include <iostream>

Route::Route()
{
}

Route::Route(JsonData & data): _data(data)
{
}

Route::Route(const Route &src): _data(src._data)
{
}

Route& Route::operator=(const Route &src)
{
	_data = src._data;
	return *this;
}

Route::~Route()
{
}

bool	Route::bad()
{
	return _data.empty();
}

t_strVec &	Route::getAllowedMethods() const
{
	return _data["methods"].stringArray();
}


int	Route::isMethodAllowed(std::string method) const
{
	if (_data["methods"].empty())
		return false;

	t_strVec &	methods = getAllowedMethods();
	for (t_strVec::iterator it = methods.begin();
		it != methods.end(); it++)
		if (it->compare(method) == 0)
			return true;
	return false;
}

// std::string	Route::getLocalPath(URI uri)
std::string	Route::getLocalPath(std::string ressourcePath) //en attendant uri
{
	//test
	return _data["root"].string() + ressourcePath;
}

std::string Route::getRoot() // @JA a checker
{
	return _data["root"].string();
}

bool	Route::isDirectoryListingEnabled() // @JA a checker
{
	if (_data["directory_listing"].empty())
		return false;
	return _data["directory_listing"].primitive();
}
