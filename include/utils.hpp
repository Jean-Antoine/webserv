/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:01:31 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/10 10:16:29 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILS_HPP__
# define __UTILS_HPP__
# include "colors.hpp"
# include <vector>
# include <string>
# include <sstream>
# include <iostream>
# include <cerrno>
# include <cstring>
# include <cstdlib>
# include <fcntl.h>
# include <ctime>
# include <cctype>
# include <deque>

typedef std::vector < std::string > t_strVec;

std::string		to_string(long i);
void			rmLeadSpaces(std::string & str);
int				error(const char *prefix);
std::string		concatStrVec(t_strVec strs, std::string sep, bool ignoreEmpty);
int 			convertHexa(const std::string &hexa, size_t &res);
std::string 	getDate();

template <typename Container>
Container	split(const std::string & data, std::string delimiter)
{
	size_t		prev = 0;
	size_t		pos = 0;
	Container	lines;
	
	if (data.empty() || delimiter.empty())
		return lines;
	while (pos != std::string::npos)
	{
		pos = data.find(delimiter, prev);
		lines.push_back(data.substr(prev, pos - prev));
		prev = pos + delimiter.length();
	}
	return lines;
}

#endif
