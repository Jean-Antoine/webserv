/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:41:07 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/09 18:42:56 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"
#include <iostream>

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
