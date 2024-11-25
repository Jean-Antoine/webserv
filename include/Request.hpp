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

# include <map>
# include <cstring>
# include <streambuf>
# include <sstream>
# include <string>

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
	std::string		path;
	std::string 	httpVersion;
}					t_reqLine;

typedef std::map <std::string, std::string> t_headers;

class RequestGet;
class RequestPost;
class RequestDelete;

class Request
{
	public:
		virtual ~Request() {};
		Request() {};
		Request(char *buffer);

		int					parseReqLine();
		int					parseHeaders();
		int					parseBody();
		t_reqLine 			&getReqLine() {return _reqLine;};
		void 				setValid(bool valid) { _isValid = valid; }
		Request				*getReq();

	private:
		bool				_isValid;
		std::istringstream	_bufferStream;
		t_reqLine			_reqLine;
		t_headers			_headers;
		std::string			_body;

		RequestGet			*getGetRequest();
		RequestPost			*getPostRequest();
		RequestDelete		*getDeleteRequest();

		int					validateMethod(std::string method);
		int					validatePath(std::string path);
		int					validateHttpVersion(std::string httpVersion);
		
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
