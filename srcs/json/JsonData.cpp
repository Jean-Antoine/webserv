/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonData.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:04:11 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/15 12:53:45 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "JsonData.hpp"
# include <iostream>

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
		delete static_cast < t_obj* > (_value);
	else if (_type == STRINGARRAY)
		delete static_cast < t_strArray* > (_value);
	else if (_type == STRING)
		delete static_cast < t_str* > (_value);
	else if (_type == PRIMITIVE)
		delete static_cast < t_prim* > (_value);
	else if (_type == OBJECTARRAY)
		delete static_cast < t_objArray* > (_value);

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
		_value = new t_str(*static_cast < t_str* > (src._value));
	else if (_type == PRIMITIVE)
		_value = new t_prim(*static_cast < t_prim* > (src._value));
	else if (_type == OBJECT)
		_value = new t_obj(*static_cast < t_obj* > (src._value));
	else if (_type == OBJECTARRAY)
		_value = new t_objArray(*static_cast < t_objArray* > (src._value));
	else if (_type == STRINGARRAY)
		_value = new t_strArray(*static_cast < t_strArray* > (src._value));

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
	if (_type != OBJECT || !_value)
		return empty::data;

	t_obj *ptr = static_cast<t_obj*>(_value);
	for (t_obj::iterator it = (*ptr).begin(); it != (*ptr).end(); it++)
		if (it->key().compare(key) == 0)
			return *it;
	return empty::data;
}

JsonData& JsonData::operator[](int idx) const
{
	if (_type != OBJECTARRAY || !_value)
		return empty::data;

	t_objArray* ptr = static_cast<t_objArray*>(_value);
	if ((unsigned long) idx >= ptr->size())
		return empty::data;
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

int	JsonData::size() const
{
	if (_type != OBJECTARRAY && _type != OBJECT)
		return 0;
	return (static_cast < t_obj* > (_value))->size();
}

t_strArray&	JsonData::stringArray() const
{
	if (_type == STRINGARRAY)
		return *static_cast < t_strArray * > (_value);
	return empty::stringArray;
}

t_prim&	JsonData::primitive() const
{
	if (_type == PRIMITIVE)
		return *static_cast < t_prim * > (_value);
	return empty::primitive;
}

t_str&	JsonData::string() const
{
	if (_type == STRING)
		return *static_cast < t_str * > (_value);
	return empty::string;
}

namespace empty
{
	t_str		string = "";
	t_obj		object = t_obj();
	t_objArray	objectArray = t_objArray();
	t_strArray	stringArray = t_strArray();
	JsonData	data = JsonData();
	int			primitive = 0;
}
