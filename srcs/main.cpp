/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:30:50 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/11/14 15:56:29 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Server.hpp"

void parseConfig(char *file)
{
	std::cout << "Parsing config file: " << file << std::endl;
}

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Error: wrong number of arguments" << std::endl;
		std::cerr << "Usage: ./webserv [configuration file]" << std::endl;
		return 1;
	}
	(void)av;
	
	Server server;
	if (server.startListening())
		return 1;
	
	
	return 0;
}