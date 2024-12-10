/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:58:07 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/10 14:53:22 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "utils.hpp"

std::string	to_string(int i)
{
	std::ostringstream ss;

	ss << std::dec << i;
	return ss.str();
}

t_strVec	split(const std::string & data, std::string delimiter)
{
	size_t		prev = 0;
	size_t		pos = 0;
	t_strVec	lines;
	
	if (data.size() == 0)
		return lines;
	while (pos != std::string::npos)
	{
		pos = data.find(delimiter, prev);
		std::string	line = data.substr(prev, pos - prev);
		lines.push_back(line);
		prev = pos + delimiter.length();
	}
	return lines;
}

void	rmLeadSpaces(std::string & str)
{
	std::string::iterator	it = str.begin();
	
	while (it != str.end() && *it == ' ')
		str.erase(it);
}

void log(const std::string &message)
{
	std::cout << message << RESET << std::endl;
}

void testLog(const std::string &message)
{
	std::cout << ORANGE << "TEST : " << message << RESET << std::endl;
}

int putError(const std::string &errorMessage, int code) //code = 1 by default
{
	std::cerr << RED << "ERROR: " << RESET << errorMessage << std::endl;
	std::cerr << "Status code: " << code << std::endl;
	return code;
}

int error(const char *prefix)
{
	std::string out;
	
	out.append(RED);
	out.append(prefix);
	out.append(": ");
	out.append(strerror(errno));
	out.append(RESET "\n");
	std::cerr << out;
	return EXIT_FAILURE;
}

std::string getDate()
{
	char buffer[30];
	std::time_t now = std::time(0);
	std::tm* gmt = std::gmtime(&now);
	if (!gmt
		|| !std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmt))
		return "";
	return std::string(buffer);
}

std::string concatStrVec(t_strVec strs, std::string sep, bool ignoreEmpty)
{
	if (strs.empty())
		return "";

	std::string res;
	for (size_t i = 0; i < strs.size(); i++)
	{
		res.append(strs[i]);
		if ( i < strs.size() - 1 && (!ignoreEmpty || !strs[i + 1].empty()))
			res.append(sep);
	}
	return res;
}
