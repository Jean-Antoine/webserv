/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:29:41 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/27 11:40:06 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JsonParser.hpp"
#include <iostream>

JsonParser::JsonParser(char *path)
{
	_failed = false;
	try
	{
		_fd.open(path, std::ifstream::in);
		if (_fd.bad() || _fd.fail())
			throw std::invalid_argument("Bad file input.");
		_buf = _fd.rdbuf();
		_root = JsonData("root", OBJECT, new t_obj(parseObj()));
	}
	catch(const std::exception& e)
	{
		_failed = true;
		std::cerr << RED << e.what() << '\n' << RESET;
	}	
	_fd.close();
}

JsonParser::~JsonParser()
{
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

	key = parseStr();
	if (nextChar(true) != ':')
		throw std::invalid_argument("Bad format.");
	else if (std::isalnum(nextChar(false)))
		return JsonData(key, PRIMITIVE, new t_prim(parsePrim()));
	else if (nextChar(false) == '"')
		return JsonData(key, STRING, new t_str(parseStr()));
	else if (nextChar(false) == '{')
		return JsonData(key, OBJECT, new t_obj(parseObj()));
	else if (nextChar(true) == '[')
	{
		if (nextChar(false) == '"')
			return JsonData(key, STRINGARRAY,
				new t_strArray(parseStrArray()));
		if (nextChar(false) == '{')
			return JsonData(key, OBJECTARRAY,
				new t_objArray(parseObjArray()));
	}
	throw std::invalid_argument("Bad format.");
	return JsonData();
}

t_obj	JsonParser::parseObj()
{
	t_obj		obj;
	
	if (nextChar(true) != '{')
		throw std::invalid_argument("Bad format.");
	while (true)
	{
		JsonData	data = parse();

		obj.push_back(data);
		if (nextChar(false) == '}')
			break;
		if (nextChar(true) != ',')
			throw std::invalid_argument("Bad format.");
	}
	nextChar(true);
	return obj;
}

t_objArray	JsonParser::parseObjArray()
{
	t_objArray	array;

	while (true)
	{
		t_obj	obj = parseObj();
		
		array.push_back(JsonData("", OBJECT, new t_obj(obj)));
		if (nextChar(false) == ']')
			break;
		if (nextChar(true) != ',')
			throw std::invalid_argument("Bad format.");
	}
	nextChar(true);
	return array;
}

t_strArray	JsonParser::parseStrArray()
{
	t_strArray	array;	

	while (true)
	{	
		array.push_back(parseStr());
		if (nextChar(false) == ']')
			break;
		if (nextChar(true) != ',')
			throw std::invalid_argument("Bad format.");
	}
	nextChar(true);
	return array;
}

char	JsonParser::nextChar(bool bump)
{
	while (std::isspace(_buf->sgetc()))
		_buf->sbumpc();
	if (bump)
		return _buf->sbumpc();
	return _buf->sgetc();
}

std::string	JsonParser::parseStr()
{
	std::string	str;
	
	if (nextChar(true) != '"')
		throw std::invalid_argument("Bad format.");
	while (_buf->sgetc() != '"')
		str.push_back(_buf->sbumpc());
	_buf->sbumpc();
	return str;
}

int	JsonParser::parseBool()
{
	std::string out;

	while (std::isalnum(nextChar(false)))
		out.push_back(nextChar(true));		
	if (out.compare("true") == 0)
		return 1;
	if (out.compare("false") == 0)
		return 0;
	throw std::invalid_argument("Bad format.");
}

int	JsonParser::parsePrim()
{
	if (nextChar(false) == 't'
	|| nextChar(false) == 'f')
		return parseBool();
	if (!isNumeric(nextChar(false)))
		throw std::invalid_argument("Bad format.");
	std::string	value;
	while (isNumeric(_buf->sgetc()))
		value.push_back(_buf->sbumpc());
	return std::strtod(value.data(), NULL);
}

const JsonData&	JsonParser::getData() const
{
	return _root;
}

bool	JsonParser::failed() const
{
	return _failed;
}

JsonData &	JsonParser::operator[](const char* key) const
{
	return _root[key];
}

JsonData &	JsonParser::operator[](const int idx) const
{
	return _root[idx];
}