/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:37:17 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/28 08:40:31 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__
# define BACKLOG		100
# define MAX_EVENTS		100
# define BUFFER_SIZE	4096
# include "utils.hpp"
# include "Config.hpp"
# include "Req.hpp"

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