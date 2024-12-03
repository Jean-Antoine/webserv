/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:54:17 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/02 15:44:40 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AMETHOD_HPP
# define AMETHOD_HPP
# include "Request.hpp"
# include "Config.hpp"

class AMethod
{
	private:
		Config &				_config;
		t_method				_method;
		URI						_uri;
		std::string				_httpVersion;
		t_headers				_headers;
		t_body					_body;
	public:
								AMethod(Config & config,  Request & request);
		virtual 				~AMethod() {};
		virtual std::string		response() = 0;

		virtual int				validateMethod();
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