/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:54:17 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/11/29 15:53:10 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AMETHOD_HPP
# define AMETHOD_HPP

# include "Request.hpp"
# include "Config.hpp"



class AMethod
{
	private:
		t_reqLine	_reqLine;
		t_headers	_headers;
		t_body		_body;
		Config		_config;
		std::string	_response;
		
	public:
		AMethod(Config& config,  Request &request);
		AMethod() {}; 
		virtual ~AMethod() {};
		virtual std::string	response() = 0;

		virtual int			validateMethod();
		int					validateReqURI();
		int					validateHttpVersion();
};


#include "Invalid.hpp"

#endif


// class Get: public AMethod {};
// class Post: public AMethod {};
// class Delete: public AMethod {};
// class Invalid: public AMethod {};