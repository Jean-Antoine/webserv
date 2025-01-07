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
		bool				_complete;
		bool				_parsingFailed;
		bool				_isCgiOut;
	public:
							Request();
							Request(const char *buffer, bool isCgiOut);
							Request(const Request & req);
		Request& 			operator=(const Request & src);
							~Request();
		

		//PARSERS
		int					parseRequest();
		int					parseReqLine();
		int					parseHeader(size_t	lineIdx);
		int					parseBody(size_t lineIdx);
		bool				isEndOfChunks(int lineIdx) const;
		bool				isLastLine(size_t lineIdx) const;
		bool				isValidChunk(size_t lineIdx) const;
		int					addChunks(size_t lineIdx);
		int					addNewChunks(const char *buffer);
		int 				parsingFail(const std::string & errorMessage);

		//GETTERS	
		bool				complete() {return _complete;};
		bool				getParsingFailed() {return _parsingFailed;};
		const std::string	getMethod() const {return _method;};
		const URI &			getURI() const {return _uri;};
		const std::string &	getHttpVersion() const {return _httpVersion;};
		const t_headers &	getHeaders() const {return _headers;};
		const std::string &	getHeader(const char *key) const;
		const t_body &		getBody() const {return _body;};
		const t_strVec &	getBufferLines() const {return _bufferLines;};

		//RESPONSE
		std::string			response(Config *config);
		bool				keepAlive();
};

#endif
