/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:27:11 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/12 09:40:40 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__
# include "JsonParser.hpp"
# include "URI.hpp"
# include "Route.hpp"

class URI;
class Config
{
	private:
		JsonData			_data;
	public:
							Config();
							Config(const JsonData& Data);
		Config &			operator=(const Config & src);
							~Config();
		const char*			host() const;
		int					port() const;
		int					check();
		Route				getRoute(URI & uri);
};

#endif