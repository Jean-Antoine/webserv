/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:18:47 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/21 15:39:14 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MESSAGE_HPP__
# define __MESSAGE_HPP__
# define CRLF "\r\n"
# define SESSION_MAX_AGE 10
# include <string>
# include <map>
# include "utils.hpp"
# include <deque>
# include "Chunk.hpp"
# include "JsonData.hpp"

typedef struct s_cookie
{
	std::string	_name;
	std::string	_value;
	std::string	_expires;
	double		_maxAge;
	std::string	_path;
	std::string	_domain;
} t_cookie;

typedef	std::pair <std::string, std::string>	t_header;
typedef std::map <std::string, std::string>		t_headers;
typedef std::string								t_body;
typedef std::map < std::string, t_cookie >		t_cookies;

class Chunk;
class Message
{
	protected:
		t_headers			_headers;
		t_cookies			_cookies;
		t_body				_body;
		bool				_fail;
		bool				_complete;
		int					parseHeader(std::string	&line);
		int					parseHeaders(t_lines &lines);
		int					parseBody(t_lines &lines);
		int					parseCookies(std::string &value);
	public:
							Message();
							Message(t_lines lines);
							Message(std::string raw, bool skipFirstLine);
							Message(std::string raw);
							Message(const Message &src);
							~Message();
		Message&			operator=(const Message &src);
		Message				operator+(const Message &src);
		Message&			operator+=(const Message &src);
		Message				operator+(const Chunk &src);
		Message&			operator+=(const Chunk &src);
		int					addNewChunks(const char *buffer);
		void				setHeader(const std::string & key, const std::string & value);
		const std::string &	getHeader(const char *key) const;
		t_cookie			getCookie(const char *key) const;
		void				setCookie(std::string name, std::string value);
		void				setCookie(t_cookie & cookie);
		std::string			getSession() const;
		std::string 		setSession();
		double				getContentLength() const;
		bool				isHeaderSet(const char *key) const;
		void				setBody(const char *str);
		void				setBody(std::string str);
		const std::string &	getBody() const;
 		bool				fail() const;
		bool				complete() const;
		bool				keepAlive() const;
};

#endif