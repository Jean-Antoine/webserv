/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:37:17 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/29 08:26:15 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__
# define BACKLOG		100
# define MAX_EVENTS		100
# define BUFFER_SIZE	4096
# include "utils.hpp"
# include "Config.hpp"
# include "Request.hpp"
 
typedef std::map < int, Request >	t_clients;

class Server
{
	private:
		Config&				_config;
		int					_socket;
		int					_epoll;
		struct addrinfo		*_address;
		t_clients			_clients;
	public:
							Server(Config& Config);
							~Server();
		int					setup();
		void				getAdress();
		int					addToPoll(int fd);
		int					acceptConnection();
		int					handleClient(int fd);
		int					setNonBlocking(int fd);
		int					error(char const* prefix);
		int					run();
};

#endif