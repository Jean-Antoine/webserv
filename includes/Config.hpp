/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:27:11 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/19 08:43:19 by jeada-si         ###   ########.fr       */
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
# define GREEN  "\e[1;32m"
# define YELLOW "\e[1;33m"
# define RED    "\e[1;31m"
# define BLUE   "\e[1;34m"
# define PINK   "\e[1;35m"
# define RESET  "\e[0m"

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

enum jsonDataType { OBJECT, ARRAY, STRING, PRIMITIVE };

typedef struct s_json_data
{	
	std::string											key;
	enum jsonDataType									type;
	std::string											string;
	std::vector < struct s_json_data >					object;
	std::vector < std::vector <struct s_json_data >	>	array;
	int													primitive;
}	t_json_data;

class Config
{
	private:
		t_json_data							data;
	public:
											Config(char *configFilePath);
											~Config();
};

#endif