/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:54:17 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/03 08:57:39 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AMETHOD_HPP
# define AMETHOD_HPP
# include "Request.hpp"
# include "Config.hpp"
# include "URI.hpp"

class AMethod
{
	private:
		Config &				_config;
		t_method				_method;
		URI						_uri;
		std::string				_httpVersion;
		t_headers				_headers;
		t_body					_body;
		t_response				_response;
		Config 					_config;		
		
		int						setResponseCode(int code, std::string response);

	public:
								AMethod(Config & config,  Request & request);
		virtual 				~AMethod() {};
		std::string				buildResponse();
		virtual std::string		getResponse() = 0;

		bool					isValid();
		int						validateMethod();
		int						validateReqURI();
		int						validateHttpVersion();
		std::string				errorResponse();
		int						isValid();
};

# include "Invalid.hpp"

#endif

// class Get: public AMethod {};
// class Post: public AMethod {};
// class Delete: public AMethod {};
// class Invalid: public AMethod {};