/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:00:12 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/11/25 13:26:58 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Tuto serveur simple: https://osasazamegbe.medium.com/showing-building-an-http-server-from-scratch-in-c-2da7c0db6cb7

#include "Server.hpp"
#include "logs.hpp"

const int BUFFER_SIZE = 30720; //a mettre dans un .hpp / ailleurs ..?

Server::Server(): _host("0.0.0.0"), _port(8081), _socketAddress_len(sizeof(_socketAddress))
{
	_socketAddress.sin_family = AF_INET;
	_socketAddress.sin_port = htons(_port);
	_socketAddress.sin_addr.s_addr = INADDR_ANY; 
	if (startServer())
	{
		stopServer();
		throw std::runtime_error("Failed to start server");//voir comment gerer les erreurs
		// {
		// 	std::ostringstream ss;
		// 	ss << "Failed to start server with PORT: " << ntohs(m_socketAddress.sin_port);
		// 	log(ss.str());
		// }
	}
	testLog("Server started");
}

Server::~Server()
{
	stopServer();
}

int	Server::startServer()
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0)
		return putError("Failed to create socket");
	int opt = 1;
	setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	std::ostringstream ss;
	if (bind(_socket, (sockaddr *)&_socketAddress, _socketAddress_len) < 0)
		return putError("Failed to bind socket");
	return 0;
}

void Server::stopServer()
{
	close(_socket);
}

int Server::acceptConnection()
{
	// std::ostringstream ss;
	// ss 	<< "ACCEPT CONNECTION" << std::endl
	// 	<< "socket = " << _socket << std::endl
	// 	<< "socketAddress = " << _socketAddress.sin_addr.s_addr << std::endl
	// 	<< "socketAddress_len = " << _socketAddress_len << std::endl
	// 	<< "port = " << ntohs(_socketAddress.sin_port) << RESET << std::endl;
	// testLog(ss.str());
	_newSocket = accept(_socket,(sockaddr *)&_socketAddress, &_socketAddress_len);
	if (_newSocket < 0)
		return putError("Failed to accept incoming connection");
	return 0;
}

int Server::startListening()
{
	if (listen(_socket, 20) < 0) //backlog TBD..? + gerer les erreurs
		return	putError("Listen failed");
	std::ostringstream ss;
	ss	<< GREEN << "Listening on ADDRESS: " 
		<< inet_ntoa(_socketAddress.sin_addr) //inet_ntoa forbidden
		<< " PORT: " << ntohs(_socketAddress.sin_port) << std::endl;
	log(ss.str());
	
	int bytesReceived;
	// while (true)
	for (int i = 0; i < 5; i++) //test
	{
		log(WHITE + "------ Waiting for a new connection ------\n");
		if (acceptConnection())
			return 1;
		char buffer[BUFFER_SIZE] = {0};
		bytesReceived = read(_newSocket, buffer, BUFFER_SIZE);
		if (bytesReceived < 0)
			return putError("Failed to read incoming message");
		log(CYAN + "> Request received from client:");

		Request *request = parseRequest(buffer);
		// Request *request = 0;
		if (sendResponse(*request))
			return 1;
		close(_newSocket);
	}
	return 0;
}

int Server::sendResponse(Request &request)
{
	std::string response = buildResponse(request);
	size_t bytesSent;
	bytesSent = send(_newSocket, response.c_str(), response.size(), 0);
	
	if (bytesSent < response.size())
		return putError("Failed to send response");
	log(GREEN + "> Response sent to client\n");
	return 0;
}

std::string Server::buildResponse(Request &request)
{
	(void)request;
	std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> Youhouu </h1><p> Welcome to JA and Leontaytay's server :) </p><p> Just Shake it off! </p></body></html>";
	std::ostringstream ss;
	ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
	<< htmlFile;
	return ss.str();
}
