/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:15:41 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/15 15:29:37 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__
# include "Request.hpp"
# include "Config.hpp"
# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>
# include <unistd.h>
# define BUFFER_SIZE	4096
# include "Server.hpp"

extern int	g_run;

int	setNonBlocking(int fd);

class Client
{
	private:
		t_virtualServers		*_virtualServers;
		int						_fd;
		struct sockaddr_storage	_addr;
		socklen_t				_len;
		Request					_request;
		std::string				_host;
		std::string				_service;
	public:
								Client();
								Client(int socket, t_virtualServers *virtualServers);
								Client(const Client &src);
								~Client();
		Client&					operator=(const Client &src);
		int						getFd() const;
		int						isValid() const;
		int						closeFd();
		void					getInfo();
		const std::string &		getHost() const;
		const std::string &		getService() const;
		Config					*getConfig() const;
		int						rcvRequest();
		int						sendResponse();
		bool					keepAlive();
};

#endif