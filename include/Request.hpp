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
# include "utils.hpp"
# include "URI.hpp"
# include "Config.hpp"

typedef std::map <std::string, std::string> t_headers;
typedef std::string							t_body;
typedef std::vector<std::string> 			t_stringVector;

class Request
{
	private:
		Config				_config;
		int					_responseCode;
		t_stringVector		_bufferLines;
		t_method			_method;
		URI					_uri;
		std::string			_httpVersion;
		t_headers			_headers;
		t_body				_body;
		int					setResponseCode(int code, std::string message = "");
	public:
							Request();
							Request(char *buffer);
							Request(const Request & req);
		Request& 			operator=(const Request & src);
							~Request();
		
		int					parseRequest();
		int					parseReqLine();
		int					parseHeader(size_t	lineIdx);
		int					parseBody(size_t lineIdx);
		method				getMethod() {return _method;};
		URI &				getURI() {return _uri;};
		std::string &		getHttpVersion() {return _httpVersion;};
		t_headers &			getHeaders() {return _headers;};
		t_body &			getBody() {return _body;};
		t_stringVector &	getBufferLines() {return _bufferLines;};
		int					isInvalid();
		std::string			response();


		// int				validateMethod(std::string &method);
		// int				validateReqURI(std::string &reqURI);
		// int				validateHttpVersion(std::string &httpVersion);

};

#endif
