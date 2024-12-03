/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:37:17 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/02 10:08:21 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef __SERVER_HPP__
# define __SERVER_HPP__
# define BACKLOG		1000
# define MAX_EVENTS		1000
# include "utils.hpp"
# include "Config.hpp"
# include "Request.hpp"
# include "Client.hpp"
 
typedef std::map < int, Client >	t_clients;

class Server
{
	private:
		Config &			_config;
		int					_socket;
		int					_epoll;
		struct addrinfo		*_addr;
		t_clients			_clients;
	public:
							Server(Config & Config);
							~Server();
		int					run();
		int					setup();
		void				getAdress();
		int					addToPoll(int fd);
		int					updatePollFlag(int fd, int flag);
		int					acceptConnection();
		int					rcvRequest(int fd);
		int					sendResponse(int fd);
		int					readRequest(int fd);
};

#endif