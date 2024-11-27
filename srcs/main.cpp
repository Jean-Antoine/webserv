/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:43:33 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/27 09:04:37 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "JsonData.hpp"
# include "JsonParser.hpp"
# include "Server.hpp"
# include <signal.h>

int	g_run = true;

static void ft_stop(int code)
{
	if (code == SIGINT)
		g_run = false;
}

int	main(int ac, char **av)
{
	signal(SIGINT, ft_stop);
	(void) av;
	if (ac != 2)
	{
		std::cerr << RED "Wrong number of arguments.\n" RESET;
		return EXIT_FAILURE;
	}
	JsonParser Config(av[1]);
	
	if (Config.failed())
	{
		std::cerr << RED "Parsing failed.\n" RESET;
		return EXIT_FAILURE;
	}
	Server Server(Config["server"][0]);
	if (Server.setup())
	{
		std::cerr << RED "Something went wrong while setuping server.\n" RESET;
		return EXIT_FAILURE;
	}
	Server.run();
	return EXIT_SUCCESS;
}
