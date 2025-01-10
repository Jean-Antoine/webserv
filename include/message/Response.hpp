/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 10:11:17 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/10 16:14:00 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__
# include "Request.hpp"
# include "string"
# include <map>
# define ERROR_DIR "./default_errors/"
# include "JsonData.hpp"
# include "Message.hpp"

typedef std::map<int, std::string> t_statusMap;

class Response: public Message
{
	private:
		std::string 				_httpVersion;
		int							_code;
		std::string					_reasonPhrase;
		static t_statusMap			_statusCodes;
		static t_statusMap			initPhrases();

	public:
									Response();
									Response(const Response &src);
									~Response();
		Response&					operator=(const Response &src);
		void						setResponseCode(int code, std::string response);
		void						setError();
		std::string					getResponse();
		static const std::string&	getReasonPhrase(int statusCode);
};

#endif
