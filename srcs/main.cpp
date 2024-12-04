/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:43:33 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/04 10:48:45 by jeada-si         ###   ########.fr       */
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
	if (code == SIGINT)
		g_run = false;
}

static void	runServers(JsonData & data)
{
	std::vector < pid_t >	pids;
	
	for (int i = 0; i < data.size(); i++)
	{
		pid_t	pid;
		
		pid = fork();
		if (pid == 0)
		{
			Config	Conf(data[i]);
			Server	Server(Conf);

			if (Server.setup())
			{
				std::cout << RED "Error while setuping server no ";
				std::cout << i << RESET "\n";
				return ;
			}
			Server.run();
			return ;
		}
		pids.push_back(pid);
	}
	for (
		std::vector< pid_t >::iterator it = pids.begin();
		it != pids.end();
		it++
	)
		waitpid(*it, NULL, 0);
}

int	main(int ac, char **av)
{
	signal(SIGINT, ft_stop);
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
	runServers(Parser["server"]);
	return EXIT_SUCCESS;
}
