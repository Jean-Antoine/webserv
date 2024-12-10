/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:01:31 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/10 14:47:49 by lpaquatt         ###   ########.fr       */
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


typedef std::vector < std::string > t_str_vec;

std::string		to_string(int i);
t_strVec		split(const std::string & data, std::string delimiter);
void			rmLeadSpaces(std::string & str);
void			log(const std::string & message);
void			testLog(const std::string & message);
int				putError(const std::string & errorMessage, int code = 1);
int				error(const char *prefix);
int 			setNonBlocking(int fd);
std::string		getDate();
std::string		concatStrVec(t_str_vec strs, std::string sep, bool ignoreEmpty);


#endif
