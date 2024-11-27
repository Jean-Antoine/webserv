/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:37:17 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/27 13:45:14 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__
# include <string>
# include <iostream>
# include <iomanip>
# include <cstdlib>
# include <cstring>
# include <cstdio>
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
# include "Config.hpp"
# include "utils.hpp"
# define GREEN  		"\e[1;32m"
# define YELLOW 		"\e[1;33m"
# define RED    		"\e[1;31m"
# define BLUE   		"\e[1;34m"
# define PINK   		"\e[1;35m"
# define RESET  		"\e[0m"
# define BACKLOG		100
# define MAX_EVENTS		100
# define BUFFER_SIZE	4096

class Server
{
	private:
		Config&				_config;
		int					_socket;
		int					_epoll;
		struct addrinfo		_hints;
		struct addrinfo		*_res;
	public:
							Server(Config& Config);
							~Server();
		int					setup();
		void				getAdress();
		int					addToPoll(int fd);
		int					acceptConnection();
		int					handleClient(int fd);
		int					run();
		int					error(char const* prefix);
};

#endif