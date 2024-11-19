/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:29:18 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/19 12:08:22 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __JSONPARSER_HPP__
# define __JSONPARSER_HPP__
# include <string>
# include <iostream>
# include <iomanip>
# include <fstream>
# include <exception>
# include <cstdlib>
# define GREEN  "\e[1;32m"
# define YELLOW "\e[1;33m"
# define RED    "\e[1;31m"
# define BLUE   "\e[1;34m"
# define PINK   "\e[1;35m"
# define RESET  "\e[0m"
# include "JsonData.hpp"

class JsonParser
{
	private:
		JsonData		_root;
		std::ifstream	_fd;
		std::streambuf	*_buf;
		class badFileInput: public std::exception
		{
			const char*	what(void) const throw();
		};
		class badFormatInput: public std::exception
		{
			const char*	what(void) const throw();
		};
	public:
					JsonParser();
					JsonParser(char *path);
					~JsonParser();
		void		loop();
		JsonData	parse();
		object		parseObject();
		array		parseArray();
		char		nextCharacter(bool bump);
		std::string	getString();
		int			getPrimitive();
		JsonData	getData() const;
};

#endif