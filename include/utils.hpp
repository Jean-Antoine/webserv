/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:01:31 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/28 11:44:42 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILS_HPP__
# include <map>
# include <sstream>
# include <iostream>
# include <iomanip>
# include <fstream>
# include <string>
# include <cstdlib>
# include <cstring>
# include <cstdio>
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
# define GREEN  		"\e[1;32m"
# define YELLOW 		"\e[1;33m"
# define RED    		"\e[1;31m"
# define BLUE   		"\e[1;34m"
# define PINK   		"\e[1;35m"
# define RESET  		"\e[0m"

typedef std::vector < std::string > t_str_vec;

std::string		to_string(int i);
t_str_vec		split(const std::string & data, std::string delimiter);
void			rmLeadSpaces(std::string & str);

#endif