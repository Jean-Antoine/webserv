/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:43:33 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/21 11:28:13 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# define GREEN  "\e[1;32m"
# define YELLOW "\e[1;33m"
# define RED    "\e[1;31m"
# define BLUE   "\e[1;34m"
# define PINK   "\e[1;35m"
# define RESET  "\e[0m"
# include "JsonData.hpp"
# include "JsonParser.hpp"

int	main(int ac, char **av)
{	
	if (ac != 2)
	{
		std::cerr << RED "Wrong number of arguments.\n" RESET;
		return 1;
	}	
	
	JsonParser config(av[1]);
	return 0;
}
