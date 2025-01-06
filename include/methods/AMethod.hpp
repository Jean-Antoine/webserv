/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:54:17 by lpaquatt          #+#    #+#             */
/*   Updated: 2025/01/06 10:08:05 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AMETHOD_HPP
# define AMETHOD_HPP
# include "Request.hpp"
# include "Config.hpp"
# include "URI.hpp"
# include "path.hpp"
# include "Response.hpp"
# include "CGI.hpp"
# include <sys/wait.h> 
# define READ 0
# define WRITE 1


class AMethod
{
	protected:
		Config 					*_config;
		Route					_route;
		Request &				_request;
		Response				_response;
		static t_strVec			_implementedMethods;
		static t_strVec			_initImplementedMethods();
		bool					validateURI();
		bool					validateRoute();
		bool					validateMethod();
		bool					validateHttpVersion();
		bool					checkAllowedMethods();
		bool					isValid();
		bool					executeCgi();
		std::string				getMimeType();
	public:
								AMethod(Config * config,  Request & request);
		virtual 				~AMethod() {};
		virtual std::string		getResponse() = 0;
};

# include "Get.hpp"
# include "Post.hpp"
# include "Delete.hpp"

#endif
