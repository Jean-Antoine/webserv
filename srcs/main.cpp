/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:43:33 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/11 18:07:34 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "JsonData.hpp"
# include "JsonParser.hpp"
# include "Server.hpp"
# include "Config.hpp"
# include <signal.h>
# include <wait.h>

int	g_run = true;

static void ft_stop(int code)
{
	if (code == SIGINT || code == SIGTERM)
		g_run = false;
}

int	main(int ac, char **av)
{
	signal(SIGINT, ft_stop);
	signal(SIGTERM, ft_stop);
	if (ac != 2)
	{
		std::cerr << RED "Wrong number of arguments.\n" RESET;
		return EXIT_FAILURE;
	}
	JsonParser Parser(av[1]);
	if (Parser.failed())
	{
		std::cerr << RED "Parsing failed.\n" RESET;
		return EXIT_FAILURE;
	}
	if (Parser.getData()["server"].empty())
	{
		std::cerr << RED "Check your config file.\n" RESET;
		return EXIT_FAILURE;
	}
	Server	server(Parser.getData()["server"]);
	server.run();
	return EXIT_SUCCESS;
}
