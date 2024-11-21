/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:29:18 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/21 10:56:35 by jeada-si         ###   ########.fr       */
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

class JsonParser
{
	private:
		JsonData		_root;
		std::ifstream	_fd;
		std::streambuf	*_buf;
	public:
						JsonParser(char *path);
						~JsonParser();
		char			nextChar(bool bump);
		std::string		parseString();
		int				parsePrimitive();
		JsonData		parse();
		t_object		parseObj();
		t_objectArray	parseObjArray();
		t_stringArray	parseStrArray();
		const JsonData&	getData() const;
};

#endif