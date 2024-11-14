/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonData.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:04:11 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/14 12:58:06 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JsonData.hpp"
#include <iostream>

JsonData::JsonData()
{
	
}

JsonData::JsonData(std::string key, enum json_data_type type): _key(key), _type(type)
{
}

JsonData::~JsonData()
{
	//std::cout << BLUE "[JsonData] Destructor called" RESET << std::endl;
}

JsonData& JsonData::operator[](char *key)
{
	return _object[key];
}

object& JsonData::operator[](int idx)
{
	return _array[idx];
}
