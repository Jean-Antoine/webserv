/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:33:27 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/08 16:00:59 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "URI.hpp"
#include <iostream>

static bool	isIn(int c, std::string match)
{
	return match.find(c) != std::string::npos;
}

static bool isDigit(int c)
{
	return c >= '0' && c <= '9';
}

static bool	isAlpha(int c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static bool isAlphaNum(int c)
{
	return isDigit(c) || isAlpha(c);
}

static bool isEscaped(int c)
{
	return c == '%';
}

static int	isMark(int c)
{
	return isIn(c, "-_.!~*'()");
}

static bool isUnreserved(int c)
{
	return isAlphaNum(c) || isMark(c);
}

static bool	isReserved(int c)
{
	return isIn(c, ";/?:@&=+$,");
}

static bool	isUric(int c)
{
	return isReserved(c) || isUnreserved(c) || isEscaped(c);
}

static bool isFragment(std::string & str)
{
	for (std::string::iterator it = str.begin();
		it != str.end(); it++)
		if (!isUric(*it))
			return false;
	return true;
}

static bool	isQuery(std::string & str)
{
	return isFragment(str);
}

static bool	isPchar(int c)
{
	return isUnreserved(c) || isEscaped(c) || isIn(c, ":@&=+$,");
}

static bool	isParam(std::string & str)
{
	for (std::string::iterator it = str.begin();
		it != str.end(); it++)
		if (!isPchar(*it))
			return false;
	return true;
}

static bool isSegment(std::string & str)
{
	if (str.empty())
		return true;
	if (!isPchar(str[0]))
		return false;

	t_strVec	params = split(str, ";");
	for (t_strVec::iterator it = params.begin();
		it != params.end(); it++)
		if (!isParam(*it))
			return false;
	return true;
}

static bool isPathSegments(std::string & str)
{
	if (str.empty() || str[0] == '/')
		return false;

	t_strVec	segments = split(str, "/");
	if (segments.size() == 0)
		return false;
	for (t_strVec::iterator it = segments.begin();
		it != segments.end(); it++)
		if (!isSegment(*it))
			return false;
	return true;
}

static bool	isPort(std::string & str)
{
	if (str.empty())
		return false;
	for (std::string::iterator it = str.begin();
		it != str.end(); it++)
		if (!isDigit(*it))
			return false;
	return true;
}

static bool isIPv4adress(std::string & str)
{
 	if (str.empty() || !isDigit(str[0]) || !isDigit(*(str.end() - 1)))
		return false;
	
	t_strVec	nodes = split(str, ".");
	if (nodes.size() != 4)
		return false;
	for (t_strVec::const_iterator it = nodes.begin();
		it != nodes.end(); it++)
		{
			if (it->size() == 0 || it->size() > 3)
				return false;
			for (std::string::const_iterator itt = it->begin();
				itt != it->end(); itt++)
				if (!isDigit(*itt))
					return false;			
		}
	return true;
}

static bool	isTopLabel(std::string & str)
{
	if (str.empty()
		|| !isAlpha(str[0])
		|| !isAlpha(*(str.end() - 1)))
		return false;
	
	for (std::string::iterator it = str.begin();
		it != str.end(); it++)
		if (!isAlpha(*it) && *it != '-')
			return false;
	return true;
}

static bool	isDomainLabel(std::string & str)
{
	if (str.empty()
		|| !isAlphaNum(str[0])
		|| !isAlphaNum(*(str.end() - 1)))
		return false;
	
	for (std::string::iterator it = str.begin();
		it != str.end(); it++)
		if (!isAlphaNum(*it) && *it != '-')
			return false;
	return true;
}

static bool	isHostName(std::string str)
{
	if (str.empty()
		|| str[0] == '.')
		return false ;
	if (*(str.end() - 1) == '.')
		str.erase(str.size() - 1);

	t_strVec	nodes = split(str, ".");
	for (t_strVec::iterator it = nodes.begin();
		it != nodes.end() - 1; it++)
		if (!isDomainLabel(*it))
			return false;
	if (!isTopLabel(*(nodes.end() - 1)))
		return false;
	return true;
}

static bool	isHost(std::string & str)
{
	return isHostName(str) || isIPv4adress(str);
}

bool	URI::isHostPort(std::string & str)
{
	_host = str;
	if (isHost(str))
		return true;
	if (*(str.end() - 1) == ':')
		return false;
	
	t_strVec	nodes = split(str, ":");
	if (nodes.size() != 2)
		return false;
	_host = nodes[0];
	_port = nodes[1];		
	return isHost(nodes[0]) && isPort(nodes[1]);
}

// static bool	isRegName(std::string & str)
// {
// 	if (str.empty())
// 		return false;

// 	for (std::string::iterator it = str.begin();
// 		it != str.end(); it++)
// 		if (!isUnreserved(*it)
// 		&& !isEscaped(*it)
// 		&& !isIn(*it, "$,;:@&=+"))
// 			return false;
// 	return true;
// }

bool	URI::isAuthority(std::string & str)
{
	return isHostPort(str);// || isRegName(str);
}

static bool isScheme(std::string & str)
{
	if (str.empty() || !isAlpha(str[0]))
		return false;

	for (std::string::iterator it = str.begin();
		it != str.end(); it++)
		if (!isAlphaNum(*it)
			&& !isIn(*it, "+-."))
			return false;
	return true;
}

static bool	isRelSegment(std::string & str)
{
	if (str.empty())
		return false;

	for (std::string::iterator it = str.begin();
		it != str.end(); it++)
		if (!isUnreserved(*it)
			&& !isEscaped(*it)
			&& !isIn(*it, ";@&=+$,"))
			return false;
	return true;
}

static std::string	left(std::string & str, int sep, bool include)
{
	std::string				out;
	std::string::iterator	it = str.begin();

	while (it != str.end() && *it != sep)
		out.push_back(*it++);
	if (include && it != str.end())
		out.push_back(*it);
	return out;
}

static std::string	right(std::string & str, int sep, bool include)
{
	std::string				out;
	std::string::iterator	it = str.begin();

	while (it != str.end() && *it != sep)
		it++;
	if (!include && it != str.end())
		it++;
	while (it != str.end())
		out.push_back(*it++);
	return out;
}

bool	URI::isAbsPath(std::string & str)
{
	if (str.empty() || str[0] != '/')
		return false;
	if (str.size() == 1)
		return true;

	std::string	pathSegment = str.substr(1);
	if (!isPathSegments(pathSegment))
		return false;
	_absPath = str;
	return true;
}

bool URI::isRelPath(std::string & str)
{
	std::string	relSegment = left(str, '/', false);
	std::string	_absPath = right(str, '/', true);

	return isRelSegment(relSegment) && isAbsPath(_absPath);
}

bool	URI::isNetPath(std::string & str)
{
	if (str.size() < 3 || str[0] != '/' || str[1] != '/')
		return false;

	std::string	sub = str.substr(2);
	std::string	authority = left(sub, '/', false);
	_absPath = right(sub, '/', true);

	return isAuthority(authority)
		&& (_absPath.empty() || isAbsPath(_absPath));
}

static bool	isUricNoSlash(int c)
{
	return isUnreserved(c) || isEscaped(c) || isIn(c, ";?:@&=+$,");
}

bool	URI::isRelativeURI()
{
	_path = left(_uri, '?', false);
	_query = right(_uri, '?', false);

	return (isNetPath(_path) 
		|| isAbsPath(_path)
		|| isRelPath(_path))
		&& (_query.empty() || isQuery(_query));
}

bool	URI::isOpaquePart()
{
	if (_part.empty()  || !isUricNoSlash(_part[0]))
		return false;

	for (std::string::iterator it = _part.begin();
		it != _part.end(); it++)
		if (!isUric(*it))
			return false;
	return true;
}

bool	URI::isHierPart()
{
	_path = left(_part, '?', false);
	_query = right(_part, '?', false);

	return (isNetPath(_path) 
		|| isAbsPath(_path))
		&& (_query.empty() || isQuery(_query));
}

bool	URI::isAbsoluteURI()
{
	if (_uri.find(':') == std::string::npos)
		return false;
	_scheme = left(_uri, ':', false);
	_part = right(_uri, ':', false);

	return (isScheme(_scheme) && (isHierPart() || isOpaquePart()));
}

bool	URI::isURI()
{
	return isAbsoluteURI() || isRelativeURI();
}

static int	toChar(int a, int b)
{
	std::string base = "123456789abcdef";
	a = tolower(a);
	b = tolower(b);
	
	return (base.find(a) + 1) * 16 
		+ (base.find(b) + 1);
}

static bool	escape(std::string & str)
{
	std::string				out;
	std::string::iterator	it = str.begin();

	while (it != str.end())
	{
		if (*it == '%')
		{
			if (std::distance(++it, str.end()) < 3)
				return EXIT_FAILURE;
			out.push_back(toChar(*it++, *it));
		}
		else
			out.push_back(*it);
		it++;
	}
	str = out;
	return EXIT_SUCCESS;
}

URI::URI()
{
	
}

URI::URI(const char *uri)
{
	_uri = std::string(uri);
	_bad = !isURI();
	
	if (_absPath.empty())
		_absPath = '/';
	if (escape(_query) || escape(_absPath))
		_bad = true;
	
	Logs(PINK) < "URI: " 
		< _absPath < "\n";
}

URI::~URI()
{
	
}

bool	URI::bad() const
{
	return _bad;
}

const std::string &	URI::getPath() const
{
	return _absPath;
}

const std::string &	URI::getQuery() const
{
	return _query;
}
