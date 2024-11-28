/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 08:09:45 by jeada-si          #+#    #+#             */
/*   Updated: 2024/11/28 11:43:48 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __REQ_HPP__
# define __REQ_HPP__
# include "utils.hpp"
# define CRLF "\r\n"
# include "Config.hpp"

class Req
{
	private:
		// Config&									_config;
		bool									_badFormat;
		std::string								_data;
		t_str_vec								_lines;
		std::string								_method;
		std::string								_URI;
		std::string								_version;
		std::map < std::string, std::string >	_headers;
	public:
						Req(const char *req);
						~Req();
		void			purgeCRLF();
		void			parseReqLine();
		void			parseHeaders();
		int				bad();
		std::string		response();
		// void			parseBody();
};

#endif