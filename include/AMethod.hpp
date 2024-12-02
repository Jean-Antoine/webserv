/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:54:17 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/02 13:54:25 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AMETHOD_HPP
# define AMETHOD_HPP
# include "Request.hpp"
# include "Config.hpp"
# include "Invalid.hpp"

class AMethod
{
	private:
		t_reqLine				_reqLine;
		t_headers				_headers;
		t_body					_body;
		Config 					_config;
		std::string				_response;
	public:
								AMethod() {};
								AMethod(Config & config,  Request & request);
		virtual 				~AMethod() {};
		virtual std::string		response() = 0;

		virtual int				validateMethod();
		int						validateReqURI();
		int						validateHttpVersion();
		std::string				errorResponse();
};

#endif

// class Get: public AMethod {};
// class Post: public AMethod {};
// class Delete: public AMethod {};
// class Invalid: public AMethod {};