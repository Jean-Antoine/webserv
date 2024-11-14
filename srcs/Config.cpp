/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:27:42 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/14 10:08:07 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <iostream>

Config::Config(char *configFilePath)
{
	std::fstream	config;
	
	config.open(configFilePath, std::fstream::in);
	while (!config.eof())
	{
		int		buffer_size = 3000;
		char	buffer[buffer_size];
		
		config.getline(buffer, buffer_size);
		
	}
}

Config::~Config()
{
	
}

