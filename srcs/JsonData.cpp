/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonData.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:04:11 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/21 11:24:40 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JsonData.hpp"
#include <iostream>

JsonData::JsonData():
	_key(""),
	_type(OBJECT),
	_value(NULL)
{
}

JsonData::JsonData(std::string key, t_jsonDataType type, void* value):
	_key(key),
	_type(type),
	_value(value)
{
}

void	JsonData::clearValue()
{
	if (!_value)
		return ;

	if (_type == OBJECT)
		delete static_cast < t_object* > (_value);
	else if (_type == STRINGARRAY)
		delete static_cast < t_stringArray* > (_value);
	else if (_type == OBJECTARRAY)
		delete static_cast < t_objectArray* > (_value);
	else if (_type == STRING)
		delete static_cast < t_string* > (_value);
	else if (_type == PRIMITIVE)
		delete static_cast < t_primitive* > (_value);

	_value = NULL;
}

JsonData&	JsonData::operator=(const JsonData &src)
{
	if (&src == this)
		return *this;
	
	clearValue();
	_type = src._type;
	_key = src._key;
	
	if (!src._value)
		return *this;
		
	if (_type == STRING)
		_value = new t_string(*static_cast < t_string* > (src._value));
	else if (_type == PRIMITIVE)
		_value = new t_primitive(*static_cast < t_primitive* > (src._value));
	else if (_type == OBJECT)
		_value = new t_object(*static_cast < t_object* > (src._value));
	else if (_type == OBJECTARRAY)
		_value = new t_objectArray(*static_cast < t_objectArray* > (src._value));
	else if (_type == STRINGARRAY)
		_value = new t_stringArray(*static_cast < t_stringArray* > (src._value));
		
	return *this;
}

JsonData::JsonData(const JsonData &src)
{
	_value = NULL;
	*this = src;
}

JsonData::~JsonData()
{
	clearValue();
}

JsonData& JsonData::operator[](const char *key) const
{
	if (_type != OBJECT)
		return empty::data;

	t_object *ptr = static_cast<t_object*>(_value);
	for (t_object::iterator it = (*ptr).begin(); it != (*ptr).end(); it++)
	{
		if (it->key().compare(key) == 0)
			return *it;
	}
	return empty::data;
}

t_object& JsonData::operator[](int idx) const
{
	if (_type != OBJECTARRAY)
		return empty::object;

	t_objectArray* ptr = static_cast<t_objectArray*>(_value);
	if ((unsigned long) idx >= ptr->size())
		return empty::object;
	return (*ptr)[idx];
}

enum jsonDataType		JsonData::type() const
{
	return _type;
}

std::string	JsonData::key() const
{
	return _key;
}

int	JsonData::empty() const
{
	if (!_value)
		return true;
	return false;
}

void*	JsonData::data() const
{
	return _value;
}

t_stringArray&	JsonData::stringArray() const
{
	if (_type == STRINGARRAY)
		return *static_cast < t_stringArray * > (_value);
	return empty::stringArray;
}

t_objectArray&	JsonData::objectArray() const
{
	if (_type == OBJECTARRAY)
		return *static_cast < t_objectArray * > (_value);
	return empty::objectArray;
}

t_primitive&	JsonData::primitive() const
{
	if (_type == PRIMITIVE)
		return *static_cast < t_primitive * > (_value);
	return empty::primitive;
}

t_string&	JsonData::string() const
{
	if (_type == STRING)
		return *static_cast < t_string * > (_value);
	return empty::string;
}

t_object&	JsonData::object() const
{
	if (_type == OBJECT)
		return *static_cast < t_object * > (_value);
	return empty::object;
}

namespace empty
{
	t_string		string = "";
	t_object		object = t_object();
	t_objectArray	objectArray = t_objectArray();
	t_stringArray	stringArray = t_stringArray();
	JsonData		data = JsonData();
	int				primitive = 0;
}
