/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonData.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:13:02 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/19 13:52:08 by jeada-si         ###   ########.fr       */
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

enum jsonDataType { OBJECT, ARRAY, STRING, PRIMITIVE, ARRAYSTRING};
class JsonData;
typedef std::map < std::string, JsonData > object;
typedef std::vector < object > array;

class JsonData
{
	private:
		std::string				_key;
		enum jsonDataType		_type;
		std::string				_string;
		object					_object;
		array					_array;
		int						_primitive;
	public:
								JsonData();
		JsonData&				operator=(const JsonData& src);
								JsonData(std::string key);
								JsonData(std::string key, array array);
								JsonData(std::string key, object object);
								JsonData(std::string key, std::string string);
								JsonData(std::string key, int primitive);
								~JsonData();
		enum jsonDataType		type() const;
		int						size() const;
		JsonData& 				operator[](char *key);
		object& 				operator[](int idx);
		std::string				getKey() const;
		object					getObject() const;
		array					getArray() const;
		int						getPrimitive() const;
		std::string				getString() const;
};

std::ostream&	operator<<(std::ostream& os, const JsonData &data);
// std::ostream&	operator<<(std::ostream& os, const object &data);
// std::ostream&	operator<<(std::ostream& os, const array &data);

#endif
