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
# include <map>
# include <cstring>
# include <streambuf>
# include <sstream>
# include <string>
# include <vector>

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
	std::string		reqURI;
	std::string 	httpVersion;
}					t_reqLine;

typedef std::map <std::string, std::string> t_headers;
typedef std::vector<std::string> 			t_stringVector;

class RequestGet;
class RequestPost;
class RequestDelete;

class Request
{
	public:
		virtual ~Request() {};
		Request() {};
		Request(char *buffer);
		Request(Request &req);

		void				parseBufferLines(char *buffer);
		int					parseReqLine(std::string reqLine);
		int					parseHeader(std::string header);
		int					parseBody();
		t_reqLine 			&getReqLine() {return _reqLine;};
		t_headers			&getHeaders() {return _headers;};
		t_stringVector		&getBufferLines() {return _bufferLines;};
		Request				*getReq();

	private:
		int					_ResponseCode;
		t_stringVector		_bufferLines;
		t_reqLine			_reqLine;
		t_headers			_headers;
		std::string			_body;

		int					setResponseCode(int code, std::string message = "");

		RequestGet			*getGetRequest();
		RequestPost			*getPostRequest();
		RequestDelete		*getDeleteRequest();

		int					validateMethod(std::string method);
		int					validateReqURI(std::string reqURI);
		int					validateHttpVersion(std::string httpVersion);
		virtual std::string	response();
};


class RequestPost : public Request
{};
class RequestGet : public Request
{};
class RequestDelete : public Request
{};
class response
{};

#endif
