/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:54:17 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/11/28 20:06:15 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Config.hpp"

class AMethod
{
	private:
		t_reqLine	_reqLine;
		t_headers	_headers;
		t_body		_body;
		
	public:
		AMethod(Config& config, t_reqLine reqLine, t_headers& headers, t_body body);
		AMethod(const AMethod &src);
		AMethod& operator=(const AMethod &src);
		~AMethod();
		virtual std::string	response() = 0;

		virtual int			validateMethod();
		int					validateReqURI();
		int					validateHttpVersion();
};
