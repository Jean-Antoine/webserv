/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:27:11 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/28 11:34:00 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__
# include "JsonParser.hpp"

class Config
{
	private:
		JsonData		_data;
		std::string		_port;
	public:
						Config();
						Config(const JsonData& Data);
						Config(const JsonParser& Parser);
						~Config();
		const char*		host() const;
		int				port() const;
		int				check();
		
};

#endif