/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonData.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:04:11 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/19 14:07:38 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JsonData.hpp"
#include <iostream>

JsonData::JsonData()
{
	_key = "";
	_type = OBJECT;
	_string = "";
	_primitive = 0;
}

JsonData::JsonData(std::string key):
	_key(key),
	_type(ARRAYSTRING)
{
}

JsonData::JsonData(std::string key, array array):
	_key(key),
	_type(ARRAY),
	_array(array)
{
}

JsonData::JsonData(std::string key, object object):
	_key(key),
	_type(OBJECT),
	_object(object)
{	
}

JsonData::JsonData(std::string key, std::string string):
	_key(key),
	_type(STRING),
	_string(string)
{	
}

JsonData::JsonData(std::string key, int primitive):
	_key(key),
	_type(PRIMITIVE),
	_primitive(primitive)
{	
}

JsonData&		JsonData::operator=(const JsonData& src)
{
	this->_key = src.getKey();
	this->_type = src.type();
	this->_string = src.getString();
	this->_object = src.getObject();
	this->_array = src.getArray();
	this->_primitive = src.getPrimitive();
	return *this;
}


JsonData::~JsonData()
{	
}

JsonData& JsonData::operator[](char *key)
{
	return _object[key];
}

object& JsonData::operator[](int idx)
{
	return _array[idx];
}

enum jsonDataType		JsonData::type() const
{
	return _type;
}

int						JsonData::size() const
{
	if (_type == STRING || _type == PRIMITIVE)
		return 1;
	else if (_type == ARRAY)
		return _array.size();
	else
		return _object.size();
}

std::string	JsonData::getKey() const
{
	return (const std::string) _key;
}

object					JsonData::getObject() const
{
	return _object;
}

array					JsonData::getArray() const
{
	return _array;
}

int						JsonData::getPrimitive() const
{
	return _primitive;
}

std::string				JsonData::getString() const
{
	return _string;
}


std::ostream&	operator<<(std::ostream& os, const JsonData &data)
{
	os << data.getKey() << ": ";
	if (data.type() == STRING)
	{
		os << data.getString();
		return os;
	}
	else if (data.type() == PRIMITIVE)
	{
		os << data.getPrimitive();
		return os;
	}
	else if (data.type() == OBJECT)
	{
		object::iterator it;
		for (it = data.getObject().begin(); it != data.getObject().end(); it++)
			os << it->second << std::endl;
	}
	else if (data.type() == ARRAY)
	{
		array::iterator it;
		for (it = data.getArray().begin(); it != data.getArray().end(); it++)
		{
			object::const_iterator itt;
			for (itt = it->begin(); itt != it->end(); ++itt)
				os << itt->second << std::endl;
		}
	}
	return os;
}

// std::ostream&	operator<<(std::ostream& os, const array)