/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:27:11 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/27 13:03:00 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__
# include <string>
# include <iostream>
# include <fstream>
# include <iomanip>
# include <map>
# include <vector>
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