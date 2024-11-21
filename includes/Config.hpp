/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:27:11 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/21 11:47:16 by jeada-si         ###   ########.fr       */
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
# include "JsonData.hpp"

typedef struct s_route
{
	std::string								path;
	std::vector < std::string >				methods;
	std::string								root;
	std::string								index;
	bool									directory_listing;
	std::string								redirection;
	std::string								uploads;
	std::map < std::string, std::string >	cgi;
} t_route;

typedef struct s_server
{
	std::string								host;
	std::string								port;
	std::string								server_name;
	std::map < int, std::string >			error_page;
	std::string								client_max_body_size;
	std::map < std::string, t_route >		routes;
}	t_server;

class Config
{
	private:
		std::vector < t_server >			servers;
	public:
											Config(JsonData data);
											~Config();
};

#endif