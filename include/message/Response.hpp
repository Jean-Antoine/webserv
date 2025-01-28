/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 10:11:17 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/27 15:32:44 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__
# define ERROR_DIR "./default_errors/"
# include "Request.hpp"
# include "JsonData.hpp"
# include "Message.hpp"
# include "CGI.hpp"
# include <map>
# include <string>

typedef std::map<int, std::string> t_statusMap;

class Response: public Message
{
	private:
		std::string 				_httpVersion;
		int							_code;
		std::string					_reasonPhrase;
		static t_statusMap			_statusCodes;
		CGI							*_cgi;
		static t_statusMap			initPhrases();
	public:
									Response();
									Response(bool timeout);
									Response(const Response &src);
									~Response();
		Response&					operator=(const Response &src);
		void						setResponseCode(int code, std::string response);
		void						setError(Config &config);
		void						runCGI(const Request & request,
										const std::string & scriptPath,
										const std::string & binPath);
		std::string					getResponse(Config &config);
		static const std::string&	getReasonPhrase(int statusCode);
		bool						ready();
		CGI*						getCGI() const;
		void						clearCGI();
};

#endif
