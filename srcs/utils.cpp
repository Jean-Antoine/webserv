/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:58:07 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/21 13:28:04 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "utils.hpp"
# include "Logs.hpp"

std::string	to_string(long i)
{
	std::ostringstream ss;

	ss << std::dec << i;
	return ss.str();
}

void	trimLeadSpaces(std::string & str)
{
	if (str.empty() || !std::isspace(str[0]))
		return ;

	size_t start = 0;
	while (start < str.length() && std::isspace(str[start]))
		++start;
	str.erase(0, start);
}

void	trimTailSpaces(std::string & str)
{
	if (str.empty() || !std::isspace(*str.rbegin()))
		return ;

	size_t end = str.length() - 1;

    while (end >= 0 && std::isspace(str[end]))
        --end;
    str.erase(end + 1);
}

void	trimSpaces(std::string & str)
{
	trimLeadSpaces(str);
	trimTailSpaces(str);
}

void	rmLeadSpaces(std::string & str)
{
	std::string::iterator	it = str.begin();
	
	while (it != str.end() && *it == ' ')
		it = str.erase(it);
}

void	rmTailSpaces(std::string & str)
{
	std::string::iterator	it = str.end() - 1;

	while (it != str.begin() && *it == ' ')
		it = str.erase(it);
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
int convertHexa(const std::string &hexa, size_t &res)
{
	for (size_t i = 0; i < hexa.length(); ++i) {
		if (!std::isxdigit(hexa[i]))
			return EXIT_FAILURE;
	}

	std::istringstream hexaStream(hexa);
	hexaStream >> std::hex >> res;
	if (hexaStream.fail() || !hexaStream.eof())
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
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
