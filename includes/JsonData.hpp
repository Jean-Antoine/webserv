/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonData.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:13:02 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/14 13:29:55 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __JSONDATA_HPP__
# define __JSONDATA_HPP__
# include <map>
# include <vector>
# include <string>
# include <iostream>
# include <iomanip>
# define GREEN  "\e[1;32m"
# define YELLOW "\e[1;33m"
# define RED    "\e[1;31m"
# define BLUE   "\e[1;34m"
# define PINK   "\e[1;35m"
# define RESET  "\e[0m"

enum json_data_type { OBJECT, ARRAY, STRING, PRIMITIVE };
class JsonData;
typedef std::map < std::string, JsonData > object;

class JsonData
{
	private:
		std::string				_key;
		enum json_data_type		_type;
		std::string				_string;
		object					_object;
		std::vector < object >	_array;
		int						_primitive;
	public:
								JsonData();
								JsonData(std::string key, enum json_data_type type);
								~JsonData();
		enum json_data_type		type() const;
		int						size() const;
		void					get();
		JsonData& 				operator[](char *key);
		object& 				operator[](int idx);
};

#endif