/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:54:17 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/09 18:10:53 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AMETHOD_HPP
# define AMETHOD_HPP
# include "Request.hpp"
# include "Config.hpp"
# include "URI.hpp"
# include "path.hpp"

typedef struct	s_statusLine
{
	std::string 				httpVersion;
	int							code;
	std::string					reasonPhrase;
}								t_statusLine;

typedef	struct	s_response
{
	t_statusLine				statusLine;
	t_headers					headers;
	t_body						body;
}								t_response;

class AMethod
{

	protected:
		Config 					*_config;
		Route					_route;
		Request &				_request;
		t_response				_response;
	public:
								AMethod(Config * config,  Request & request);
		virtual 				~AMethod() {};
		virtual std::string		getResponse() = 0;
		std::string				buildResponse();
		int						setResponseCode(int code, std::string response);

		bool					checkAllowedMethods();
		bool					validateMethod();
		bool					validateReqURI();
		bool					validateHttpVersion();
		std::string				getErrorBody(int code);
		std::string				errorResponse();
		bool					isValid();
};

# include "Get.hpp"
# include "Post.hpp"
# include "Delete.hpp"
# include "Invalid.hpp"

#endif
