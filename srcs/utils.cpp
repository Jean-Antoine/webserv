/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:58:07 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/28 11:42:41 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "utils.hpp"

std::string	to_string(int i)
{
	std::ostringstream ss;

	ss << std::dec << i;
	return ss.str();
}

t_str_vec	split(const std::string & data, std::string delimiter)
{
	size_t		prev = 0;
	size_t		pos = 0;
	t_str_vec	lines;
	
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
