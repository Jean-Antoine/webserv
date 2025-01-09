/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:43:33 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/09 09:58:00 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "JsonData.hpp"
# include "JsonParser.hpp"
# include "Server.hpp"
# include "Config.hpp"
# include "Path.hpp"
# include <signal.h>
# include <wait.h>
# include "Logs.hpp"

int	g_run = true;
int	g_exitStatus = EXIT_SUCCESS;

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
		Logs(RED) << "Wrong number of arguments.\n";
		return EXIT_FAILURE;
	}
	JsonParser Parser(av[1]);
	if (Parser.failed())
	{
		Logs(RED) << "Parsing failed.\n";
		return EXIT_FAILURE;
	}
	if (Parser.getData()["server"].empty())
	{
		Logs(RED) << "Check your config file.\n";
		return EXIT_FAILURE;
	}
	Server	server(Parser.getData()["server"]);
	server.run();
	return g_exitStatus;
}

// int	main(void)
// {
// 	Path	path1("/root/path1/path3/path4/path5/../..");
// 	Path	path2("/root/");
// 	Path	path3("/root/path1");

// 	std::cout << path1 << "\n" << path2 << "\n";
// 	std::cout << "path1 in path2 ? " << path1.in(path2) << "\n";
// 	std::cout << "path2 in path1 ? " << path2.in(path1) << "\n";
// 	std::cout << "path1 + path2 " << path1 + path2 << "\n";
// 	std::cout << "path3 compare path1 " << path3.compare(path1) << "\n";
// 	std::cout << "path2 compare path1 " << path3.compare(path2) << "\n";

// 	Path	replaced = path1;
// 	Path	root("/root/path1");
// 	Path	replacement("/thisisatest");
	
// 	replaced.replace(root, replacement);
// 	std::cout << replaced << "\n";
// }
