/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:15:41 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/18 15:30:34 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__
# include "Request.hpp"
# include "Config.hpp"
# include <sys/socket.h>
# include <netdb.h>
# include <unistd.h>
# include "Logs.hpp"
# define BUFFER_SIZE	4096

extern int	g_run;

int	setNonBlocking(int fd);

class Logs;
class Client
{
	private:
		Config					*_config;
		int						_fd;
		struct sockaddr_storage	_addr;
		socklen_t				_len;
		Request					_request;
		std::string				_host;
		std::string				_service;
	public:
								Client();
								Client(int socket, Config *config);
								Client(const Client &src);
								~Client();
		Client&					operator=(const Client &src);
		int						getFd() const;
		int						isValid() const;
		int						closeFd();
		void					getInfo();
		const std::string &		getHost() const;
		const std::string &		getService() const;
		int						rcvRequest();
		int						sendResponse();
		bool					keepAlive();
};

const Logs &	operator<<(const Logs & logs, Client & clt);

#endif