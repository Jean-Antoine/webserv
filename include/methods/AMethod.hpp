/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:54:17 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/03 11:08:26 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AMETHOD_HPP
# define AMETHOD_HPP
# include "Request.hpp"
# include "Config.hpp"
# include "URI.hpp"

typedef struct	s_statusLine
{
	std::string httpVersion;
	int			code;
	std::string	reasonPhrase;
}					t_statusLine;

typedef	struct	s_response
{
	t_statusLine	statusLine;
	t_headers		headers;
	t_body			body;
}					t_response;

class AMethod
{
	private:
		Config &				_config;
		Route					_route;
		Request &				_request;
		t_response				_response;
	public:
								AMethod(Config & config,  Request & request);
		virtual 				~AMethod() {};
		virtual std::string		getResponse() = 0;
		std::string				buildResponse();
		int						setResponseCode(int code, std::string response);

		int						validateMethod();
		int						validateReqURI();
		int						validateHttpVersion();
		std::string				errorResponse();
		bool					isValid();
};
# include "Invalid.hpp"

#endif

// class Get: public AMethod {};
// class Post: public AMethod {};
// class Delete: public AMethod {};
// class Invalid: public AMethod {};