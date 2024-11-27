/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:29:18 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/27 11:40:06 by jeada-si         ###   ########.fr       */
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
# include "JsonData.hpp"
# define GREEN	"\e[1;32m"
# define YELLOW	"\e[1;33m"
# define RED	"\e[1;31m"
# define BLUE	"\e[1;34m"
# define PINK	"\e[1;35m"
# define RESET	"\e[0m"

class JsonParser
{
	private:
		JsonData			_root;
		std::ifstream		_fd;
		std::streambuf		*_buf;
		bool				_failed;
	public:
							JsonParser(char *path);
							~JsonParser();
		char				nextChar(bool bump);
		int					parseBool();
		std::string			parseStr();
		int					parsePrim();
		JsonData			parse();
		t_obj				parseObj();
		t_objArray			parseObjArray();
		t_strArray			parseStrArray();
		const JsonData &	getData() const;
		bool				failed() const;
		JsonData &			operator[](const char* key) const;
		JsonData &			operator[](const int idx) const;
};

#endif