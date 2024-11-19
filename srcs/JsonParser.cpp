/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:29:41 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/19 13:58:53 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JsonParser.hpp"
#include <iostream>

JsonParser::JsonParser()
{
	
}

JsonParser::JsonParser(char *path)
{
	_fd.open(path, std::ifstream::in);
	if (_fd.bad() || _fd.fail())
		throw badFileInput();
	_buf = _fd.rdbuf();
	loop();
}

JsonParser::~JsonParser()
{
	
}

void	JsonParser::loop()
{
	object	obj = parseObject();
	_root = JsonData("root", obj);
}

static int	isNumeric(int c)
{
	if (c < '0' || c > '0' + 9)
		return false;
	return true;
}

JsonData	JsonParser::parse()
{
	std::string	key;

	key = getString();
	if (nextCharacter(true) != ':')
		throw badFormatInput();
	if (isNumeric(nextCharacter(false)))
		return JsonData(key, getPrimitive());
	if (nextCharacter(false) == '"')
		return JsonData(key, getString());
	if (nextCharacter(false) == '[')
	{
		array	arr = parseArray();
		return JsonData(key, arr);
	}
	if (nextCharacter(false) == '{')
	{
		object	obj = parseObject();
		return JsonData(key, obj);
	}
	throw badFormatInput();
	return JsonData();
}

object	JsonParser::parseObject()
{
	object		obj;
	
	if (nextCharacter(true) != '{')
		throw badFormatInput();
	while (true)
	{
		JsonData	data;
		
		data = parse();
		obj[data.getKey()] = data;
		if (nextCharacter(false) == '}')
			break;
		if (nextCharacter(true) != ',')
			throw badFormatInput();
	}
	nextCharacter(true);
	return obj;
}

array	JsonParser::parseArray()
{
	array	arr;
	
	if (nextCharacter(true) != '[')
		throw badFormatInput();
	if (nextCharacter(false) == '{')
	{
		while (true)
		{
			object	obj;
			
			obj = parseObject();
			arr.push_back(obj);
			if (nextCharacter(false) == ']')
				break;
			if (nextCharacter(true) != ',')
				throw badFormatInput();
		}
		nextCharacter(true);
		return arr;
	}
	while (true)
	{
		
	}
	return arr;
}

char	JsonParser::nextCharacter(bool bump)
{
	while (std::isspace(_buf->sgetc()))
		_buf->sbumpc();
	if (bump)
		return _buf->sbumpc();
	return _buf->sgetc();
}

std::string	JsonParser::getString()
{
	std::string	str;
	
	if (nextCharacter(true) != '"')
		throw badFormatInput();
	while (_buf->sgetc() != '"')
		str.push_back(_buf->sbumpc());
	if (!str.size())
		throw badFormatInput();
	_buf->sbumpc();
	return str;
}

int	JsonParser::getPrimitive()
{
	if (!isNumeric(nextCharacter(false))
	&& nextCharacter(false) != 't'
	&& nextCharacter(false) != 'f')
		throw badFileInput();
	
	std::string	value;
	while (isNumeric(_buf->sgetc()))
		value.push_back(_buf->sbumpc());
	return std::strtod(value.data(), NULL);
}

const char*	JsonParser::badFileInput::what() const throw()
{
	return "Bad file input";
}

const char*	JsonParser::badFormatInput::what() const throw()
{
	return "Bad format input";
}

JsonData	JsonParser::getData() const
{
	return _root;
}
