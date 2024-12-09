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

typedef std::map <std::string, std::string> t_headers;
typedef std::string							t_body;

class Config;
class Request
{
	private:
		t_strVec			_bufferLines;
		std::string			_method;
		URI					_uri;
		std::string			_httpVersion;
		t_headers			_headers;
		t_body				_body;
		std::string			_path; //test avant uri
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
		std::string			getMethod() {return _method;};
		URI &				getURI() {return _uri;};
		std::string			getPath() {return _path;}; //test avant uri
		std::string &		getHttpVersion() {return _httpVersion;};
		t_headers &			getHeaders() {return _headers;};
		t_body &			getBody() {return _body;};
		t_strVec &			getBufferLines() {return _bufferLines;};
		std::string			response(Config *config);
		bool				keepAlive();

		// int				validateMethod(std::string &method);
		// int				validateReqURI(std::string &reqURI);
		// int				validateHttpVersion(std::string &httpVersion);

};

#endif
