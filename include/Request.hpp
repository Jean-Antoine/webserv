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
// #define MAXLEN_GET_REQUEST 2048

enum method
{
	GET,
	POST,
	DELETE,
	INVALID
};

typedef struct s_reqLine
{
	method			method;
	URI				reqURI;
	std::string 	httpVersion;
}					t_reqLine;

typedef std::map <std::string, std::string> t_headers;
typedef std::string							t_body;
typedef std::vector<std::string> 			t_stringVector;

class Request
{
	private:
		int					_ResponseCode;
		t_stringVector		_bufferLines;
		t_reqLine			_reqLine;
		URI					_uri;
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
		t_reqLine &			getReqLine() {return _reqLine;};
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
