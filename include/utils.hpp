/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:01:31 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/06 09:10:12 by jeada-si         ###   ########.fr       */
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

typedef enum method
{
	GET,
	POST,
	DELETE,
	INVALID
}	t_method;

typedef std::string t_strMethods[3];

typedef std::vector < std::string > t_strVec;

std::string		to_string(int i);
t_strVec		split(const std::string & data, std::string delimiter);
void			rmLeadSpaces(std::string & str);
void			log(const std::string & message);
void			testLog(const std::string & message);
int				putError(const std::string & errorMessage, int code = 1);
int				error(const char *prefix);
std::string 	getDate();

#endif
