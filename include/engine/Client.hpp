/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:15:41 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/23 16:33:40 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__
# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>
# include <unistd.h>
# define BUFFER_SIZE	4096
# define RCV_TIMEOUT 	2
# include "Server.hpp"
# include "Response.hpp"
# include "Request.hpp"

typedef std::map < std::string, int >		t_sessions;

extern int	g_run;

int	setNonBlocking(int fd);

class Response;
class Request;
class Client
{
	private:
		int						_fd;
		struct sockaddr_storage	_addr;
		socklen_t				_len;
		std::string				_host;
		std::string				_service;
		
		t_virtualServers		*_virtualServers;
		std::string				_received;
		Request					_request;
		Response				_response;
		
		time_t					_start;
		bool					_timeout;

		static t_sessions		_sessions;
		std::string				_sessionId;
		
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
		const std::string &		getSessionId() const;
		Config&					getConfig() const;
		int						handleTLSConnection();
		int						checkRecv();
		int						rcvRequest();
		void					setResponse();
		int						sendResponse();
		bool					keepAlive();
		void					incrementSessionReqCnt(const std::string & id);
		int 					getSessionReqCnt(const std::string & id);
		bool					complete() const;
		bool					timeout() const;
};

#endif