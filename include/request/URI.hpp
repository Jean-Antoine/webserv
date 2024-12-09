/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:29:38 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/09 18:34:38 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __URI_HPP__
# define __URI_HPP__
# include "utils.hpp"

class URI
{
	private:
		std::string		_raw;
		std::string		_scheme;
		std::string		_absPath;
		std::string		_part;
		std::string		_path;
		std::string		_query;
		std::string		_authority;
		bool			isNetPath(std::string & str);
		bool			isHierPart(std::string & str);
		bool			isRelativeURI(std::string & str);
		bool			isAbsoluteURI(std::string & str);
		bool			isURI(std::string & str);
	public:
						URI();
						URI(const char* uri);
						~URI();
};

#endif