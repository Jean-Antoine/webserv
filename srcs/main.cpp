/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:43:33 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/25 12:24:44 by jeada-si         ###   ########.fr       */
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
