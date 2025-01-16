/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 11:39:02 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/11/22 14:54:54 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP
# define CRLF "\r\n"
# include "Config.hpp"
# include "URI.hpp"
# include "utils.hpp"
# include <map>
# include <string>
# include "Message.hpp"

typedef std::map <std::string, std::string> t_headers;
typedef std::string							t_body;

class Config;
class Request: public Message
{
	private:
		std::string			_method;
		URI					_uri;
		std::string			_httpVersion;
	public:
							Request();
							Request(const char *buffer);
							Request(const Request & req);
		Request& 			operator=(const Request & src);
							~Request();
		const std::string&	getMethod() const;
		const URI&			getURI() const;
		const std::string&	getHttpVersion() const;
		int					parseReqLine(std::string &line);
		std::string			response(Config &config);
		bool				keepAlive();
};

#endif
