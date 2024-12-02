/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:01:31 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/02 18:59:53 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILS_HPP__
# define __UTILS_HPP__
# include <map>
# include <sstream>
# include <iostream>
# include <iomanip>
# include <fstream>
# include <string>
# include <cstdlib>
# include <cstring>
# include <cstdio>
# include <ctime>
# include <vector>
# include <errno.h>
# include <unistd.h>
# include <fcntl.h>
# include <unistd.h>
# include <netdb.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/epoll.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# define RED		"\033[1;31m"
# define GREEN		"\033[1;32m"
# define CYAN		"\033[1;36m"
# define MAGENTA	"\033[1;35m"
# define YELLOW		"\033[1;33m"
# define GREY		"\033[1;30m"
# define WHITE		"\033[1;37m"
# define ORANGE		"\033[1;38;5;208m"
# define PURPLE		"\033[1;38;5;129m"
# define BLUE		"\033[1;34m"
# define RESET		"\033[0m"
# define PINK   	"\e[1;35m"

typedef std::vector < std::string > t_str_vec;

std::string		to_string(int i);
t_str_vec		split(const std::string & data, std::string delimiter);
void			rmLeadSpaces(std::string & str);
void			log(const std::string & message);
void			testLog(const std::string & message);
int				putError(const std::string & errorMessage, int code = 1);
int				error(const char *prefix);
int 			setNonBlocking(int fd);
std::string 	getDate();

#endif
