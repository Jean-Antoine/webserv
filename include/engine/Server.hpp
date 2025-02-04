/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:37:17 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/28 10:12:58 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__
# define BACKLOG		1000
# define MAX_EVENTS		1000
# include <map>
# include <string>
# include <sys/epoll.h>

class Config;
class JsonData;
class Client;
class CGI;
typedef std::string									t_host;
typedef int											t_port;
typedef	std::pair < t_host, t_port >				t_hostPort;
typedef int											t_socket;
typedef std::string									t_serverName;
typedef std::map < t_hostPort, t_socket >			t_sockets;
typedef std::map < t_serverName, Config >			t_virtualServers;
typedef std::map < t_socket, t_virtualServers >		t_servers;
typedef std::map < t_socket, Client >				t_clients;
typedef int											t_pipe;
typedef std::map < t_pipe, CGI * >					t_cgis;

class Server
{
	private:
		int					_epoll;
		t_sockets			_sockets;
		t_servers			_servers;
		t_clients			_clients;
		t_cgis				_cgis;
	public:
							Server(const JsonData & data);
							~Server();
		int					run();
		t_socket			addListener(t_host host, t_port port);
		void				addVirtualServer(t_socket socket, Config &config);
		int					addToPoll(t_socket fd);
		int					removeFromPoll(int fd);
		int					acceptConnection(t_socket fd);
		void				rcvRequest(t_socket fd);
		void				sendResponse(t_socket fd);
		void				rmClient(t_socket fd);
		void				rmCGI(Client &client);
		void				addCGI(Client &client);
		bool				isServer(t_socket fd) const;
		bool				isClient(t_socket fd) const;
		bool				isCGI(t_socket fd) const;
		void				handleCgiEvents(int count, epoll_event *events);
		void				handleClientEvents(int count, epoll_event *events);
};

#endif