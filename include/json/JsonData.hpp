/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonData.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:13:02 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/17 15:58:35 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __JSONDATA_HPP__
# define __JSONDATA_HPP__
# include "utils.hpp"

typedef enum jsonDataType 
{
	OBJECT,
	OBJECTARRAY,
	STRING,
	PRIMITIVE,
	STRINGARRAY
}	t_jsonDataType;

class JsonData;
typedef std::string					t_str;
typedef std::vector < JsonData >	t_obj;
typedef int							t_prim;
typedef std::vector < JsonData >	t_objArray;
typedef std::vector < t_str >		t_strArray;

class JsonData
{
	private:
		std::string		_key;
		t_jsonDataType	_type;
		void*			_value;
		void			clearValue();
	public:
						JsonData();
						JsonData(const JsonData &src);
		JsonData &		operator=(const JsonData& src);
						JsonData(
							std::string key,
							t_jsonDataType type,
							void* value);
						~JsonData();
		t_jsonDataType	type() const;
		std::string		key() const;
		int				empty() const;
		void*			data() const;
		int				size() const;
		JsonData &		operator[](const char *key) const;
		JsonData &	 	operator[](int idx) const;
		t_strArray &	stringArray() const;
		t_prim &		primitive() const;
		t_str &			string() const;
};

namespace empty
{
	extern t_str			string;
	extern t_obj			object;
	extern t_objArray		objectArray;
	extern t_strArray		stringArray;
	extern JsonData			data;
	extern int				primitive;
};

#endif
