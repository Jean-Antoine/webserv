/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:43:33 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/25 13:48:32 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "JsonData.hpp"
# include "JsonParser.hpp"

int	main(int ac, char **av)
{	
	if (ac != 2)
	{
		std::cerr << RED "Wrong number of arguments.\n" RESET;
		return 1;
	}
	JsonParser Config(av[1]);
	if (Config.failed())
		std::cerr << RED "Parsing failed.\n" RESET;
	// std::cout << config.getData();
	return 0;
}

// #include <iostream>
// #include "Server.hpp"

// void parseConfig(char *file)
// {
// 	std::cout << "Parsing config file: " << file << std::endl;
// }

// int main(int ac, char **av)
// {
// 	if (ac != 2)
// 	{
// 		std::cerr << "Error: wrong number of arguments" << std::endl;
// 		std::cerr << "Usage: ./webserv [configuration file]" << std::endl;
// 		return 1;
// 	}
// 	(void)av;
	
// 	Server server;
// 	if (server.startListening())
// 		return 1;
	
	
// 	return 0;
// }
