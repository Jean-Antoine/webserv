/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:15:41 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/28 15:19:43 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__
# include "utils.hpp"
# include <queue>
# include "Request.hpp"

class Client
{
	private:
		int						_fd;
		Request					_request;
	public:
								Client();
								Client(const Client &src);
								~Client();
		Client&					operator=(const Client &src);
		int						getFd() const;
		Request 				getRequest() const;
};

#endif