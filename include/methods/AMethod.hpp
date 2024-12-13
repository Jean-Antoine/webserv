/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:54:17 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/13 19:14:23 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AMETHOD_HPP
# define AMETHOD_HPP
# include "Request.hpp"
# include "Config.hpp"
# include "URI.hpp"
# include "path.hpp"
# include "Response.hpp"
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
	public:
								AMethod(Config * config,  Request & request);
		virtual 				~AMethod() {};
		virtual std::string		getResponse() = 0;
		std::string				buildResponse();
		std::string				getMimeType(const std::string& path);

		bool					checkAllowedMethods();
		bool					validateMethod();
		bool					validateURI();
		bool					validateRoute();
		bool					validateHttpVersion();
		bool					isValid();
		int						execCgi(std::string &dest);
		void					createCgiEnvp(char *envp[]);
		void					createCgiArgv(char *argv[]);

};

# include "Get.hpp"
# include "Post.hpp"
# include "Delete.hpp"
# include "Invalid.hpp"

#endif
