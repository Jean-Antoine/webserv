/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 10:11:17 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/18 17:44:01 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__
# include "Request.hpp"
# include "string"
# include <map>

# define ERROR_DIR "./default_errors/"

typedef std::map<int, std::string> t_statusMap;

class Response
{
	private:
		std::string 				_httpVersion;
		int							_code;
		std::string					_reasonPhrase;
		t_headers					_headers;
		t_body						_body;
		static t_statusMap			_statusCodes;
		static t_statusMap			initPhrases();

	public:
									Response();
									Response(const Response &src);
									~Response();
		Response&					operator=(const Response &src);
		int							setResponseCode(int code, std::string response, bool printErr = true);
		void						setHeader(std::string key, std::string value);
		void						appendHeader(std::string key, std::string value);
		void						setBody(const char *str);
		void						setBody(std::string str);
		void						setError();
		std::string					getResponse();
		static const std::string&	getReasonPhrase(int statusCode);
};

#endif
