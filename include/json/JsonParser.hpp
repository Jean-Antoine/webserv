/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:29:18 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/28 08:38:47 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __JSONPARSER_HPP__
# define __JSONPARSER_HPP__
# include "utils.hpp"
# include "JsonData.hpp"

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