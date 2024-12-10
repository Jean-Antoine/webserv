/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:37:17 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/06 13:12:36 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__
# define BACKLOG		1000
# define MAX_EVENTS		1000
# include "Config.hpp"
# include "Request.hpp"
# include "Client.hpp"
# include <map>
# include <sys/epoll.h>

typedef int								t_socket;
typedef std::map < t_socket, Config >	t_servers;
typedef std::map < t_socket, Client >	t_clients;

class Server
{
	private:
		int					_epoll;
		t_servers			_servers;
		t_clients			_clients;
	public:
							Server();
							Server(const JsonData & data);
							~Server();
		int					run();
		int					addToPoll(t_socket fd);
		int					updatePollFlag(t_socket fd, int flag);
		int					acceptConnection(t_socket fd);
		int					rcvRequest(t_socket fd);
		int					sendResponse(t_socket fd);
		void				rmClient(t_socket fd);
};

#endif