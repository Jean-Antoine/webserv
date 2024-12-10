/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:29:38 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/10 12:58:41 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __URI_HPP__
# define __URI_HPP__
# include "utils.hpp"

class URI
{
	private:
		std::string		_uri;
		std::string		_scheme;
		std::string		_absPath;
		std::string		_part;
		std::string		_path;
		std::string		_query;
		std::string		_host;
		std::string		_port;
		bool			_bad;
		bool			isHostPort(std::string & str);
		bool			isAuthority(std::string & str);
		bool			isAbsPath(std::string & str);
		bool 			isRelPath(std::string & str);
		bool			isNetPath(std::string & str);
		bool			isOpaquePart();
		bool			isHierPart();
		bool			isRelativeURI();
		bool			isAbsoluteURI();
		bool			isURI();
	public:
							URI();
							URI(const char* uri);
							~URI();
		const std::string &	getPath() const;
		bool				bad() const;
};

#endif