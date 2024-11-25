/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 12:53:58 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/11/14 18:42:38 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <map>
#include <vector>
#include <sys/socket.h>
#include <exception>
#include <unistd.h>
#include <arpa/inet.h> //htons
#include <netinet/in.h>
#include <sstream>


class Request
{};
class RequestPost : public Request
{};
class RequestGet : public Request
{};
class RequestDelete : public Request
{};
class response
{};

class Route;

class Server
{
	public:
		Server();
		~Server();
		int							startListening();
		void						stopServer();

	private:
		std::string 				_name;
		std::string 				_host; //ip_address
		int 						_port;
		std::map<int, std::string>	_default_error_pages;
		// std::vector<Route>			_routes;

		int 						_socket;
		struct sockaddr_in 			_socketAddress;
		int 						_newSocket;
		unsigned int 				_socketAddress_len;
		
		int							startServer(void);
		int							acceptConnection(void);
		int							sendResponse(Request &request);
		std::string					buildResponse(Request &request);
		
};

#endif
