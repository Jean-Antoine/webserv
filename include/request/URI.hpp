/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:29:38 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/05 10:25:38 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __URI_HPP__
# define __URI_HPP__
# include "utils.hpp"

class URI
{
	private:
		std::string			_raw;
		std::string			_host;
		std::string			_port;
		std::string			_path;
		std::string			_query;
		bool				_bad;
	public:
							URI();
							URI(const char* uri);
							~URI();
		// const std::string &	getPath() const;
		// void				parseHex(const char *uri);
		// void				parseScheme(std::streambuf *buf);
		// bool				parseHost(std::streambuf *buf);
		// bool				parsePort(std::streambuf *buf);
		// void				parsePath(std::streambuf *buf);
		// void				parseQuery(std::streambuf *buf);
		// int				bad() const;
};

#endif