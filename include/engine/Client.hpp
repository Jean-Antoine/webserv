/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:15:41 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/03 09:09:52 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__
# include "utils.hpp"
# include "Request.hpp"
# include "Config.hpp"
# define BUFFER_SIZE	4096

class Client
{
	private:
		int						_fd;
		struct sockaddr_storage	_addr;
		socklen_t				_len;
		Request					_request;
		char					_host[NI_MAXHOST];
		char					_service[NI_MAXSERV];
	public:
								Client();
								Client(int socket);
								Client(const Client &src);
								~Client();
		Client&					operator=(const Client &src);
		int						getFd() const;
		// Request 				getRequest() const;
		int						isValid() const;
		int						closeFd();
		void					getInfo();
		const char*				getHost() const;
		const char*				getService() const;
		int						rcvRequest();
		int						sendResponse(Config & config);
};

std::ostream &	operator<<(std::ostream & os, Client & client);

#endif