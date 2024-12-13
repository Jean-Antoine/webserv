/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 10:11:17 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/13 11:09:20 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__
# include "Request.hpp"
# include "string"
# define ERROR_DIR "./default_errors/"

class Response
{
	private:
		std::string 	_httpVersion;
		int				_code;
		std::string		_reasonPhrase;
		t_headers		_headers;
		t_body			_body;
	public:
						Response();
						Response(const Response &src);
						~Response();
		Response&		operator=(const Response &src);
		int				setResponseCode(int code, std::string response);
		void			setHeader(std::string key, std::string value);
		void			appendHeader(std::string key, std::string value);
		void			setBody(const char *str);
		void			setBody(std::string str);
		void			setError();
		std::string		getResponse();
};

#endif