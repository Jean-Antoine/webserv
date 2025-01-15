/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:54:17 by lpaquatt          #+#    #+#             */
/*   Updated: 2025/01/15 10:42:28 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AMETHOD_HPP
# define AMETHOD_HPP
# include "Config.hpp"
# include "Request.hpp"
# include "Route.hpp"
# include "Ressource.hpp"
# include "Response.hpp"
# define READ 0
# define WRITE 1


class AMethod
{
	protected:
		Config 					*_config;
		Request &				_request;
		Route					_route;
		Ressource				_ressource;
		Response				_response;
		bool					validateURI();
		bool					validateRoute();
		bool					validateMethod();
		bool					validatePayLoad();
		bool					validateHttpVersion();
		bool					checkAllowedMethods();
		bool					executeCgi();
		std::string				getMimeType();
	public:
								AMethod(Config * config,  Request & request);
		virtual 				~AMethod() {};
		bool					isValid();
		std::string				getInvalidResponse();
		virtual std::string		getResponse() = 0;
};

# include "Get.hpp"
# include "Post.hpp"
# include "Delete.hpp"

#endif
