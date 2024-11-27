/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonData.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:13:02 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/25 14:10:36 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __JSONDATA_HPP__
# define __JSONDATA_HPP__
# include <vector>
# include <string>
# include <iostream>
# include <iomanip>

typedef enum jsonDataType 
{
	OBJECT,
	OBJECTARRAY,
	STRING,
	PRIMITIVE,
	STRINGARRAY
}	t_jsonDataType;

class JsonData;
typedef std::string					t_string;
typedef std::vector < JsonData >	t_object;
typedef int							t_primitive;
typedef std::vector < JsonData >	t_objectArray;
typedef std::vector < t_string >	t_stringArray;

class JsonData
{
	private:
		std::string				_key;
		t_jsonDataType			_type;
		void*					_value;
		void					clearValue();
	public:
								JsonData();
								JsonData(const JsonData &src);
		JsonData&				operator=(const JsonData& src);
								JsonData(
									std::string key,
									t_jsonDataType type,
									void* value);
								~JsonData();
		t_jsonDataType			type() const;
		std::string				key() const;
		int						empty() const;
		void*					data() const;
		int						size() const;
		JsonData&				operator[](const char *key) const;
		JsonData&	 			operator[](int idx) const;
		t_stringArray&			stringArray() const;
		t_primitive&			primitive() const;
		t_string&				string() const;
};

namespace empty
{
	extern t_string			string;
	extern t_object			object;
	extern t_objectArray	objectArray;
	extern t_stringArray	stringArray;
	extern JsonData			data;
	extern int				primitive;
};

#endif
