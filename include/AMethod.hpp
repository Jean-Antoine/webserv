/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:54:17 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/02 19:00:08 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AMETHOD_HPP
# define AMETHOD_HPP
# include "Request.hpp"
# include "Config.hpp"
# include "utils.hpp"

typedef struct s_statusLine
{
	std::string 	httpVersion;
	int				code;
	std::string		reasonPhrase;	
}					t_statusLine;

typedef	struct s_response
{
	t_statusLine				statusLine;
	t_headers					headers;
	t_body						body;
}								t_response;

class AMethod
{
	private:
		t_reqLine				_reqLine;
		t_headers				_headers;
		t_body					_body;
		t_response				_response;
		Config 					_config;
		
		
		int						setResponseCode(int code, std::string response);

	public:
								AMethod() {};
								AMethod(Config & config,  Request & request);
		virtual 				~AMethod() {};
		std::string				buildResponse();
		virtual std::string		getResponse() = 0;

		bool					isValid();
		int						validateMethod();
		int						validateReqURI();
		int						validateHttpVersion();
		std::string				errorResponse();
};

# include "Invalid.hpp"

#endif

// class Get: public AMethod {};
// class Post: public AMethod {};
// class Delete: public AMethod {};
// class Invalid: public AMethod {};