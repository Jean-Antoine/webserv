/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:27:11 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/23 16:11:14 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__
# define MIME_TYPES_FILE "/etc/mime.types"
# define ERROR_DIR "./default_errors/"
# define CLIENT_MAX_BODY_SIZE 1048576
# include <map>
# include "JsonData.hpp"

typedef std::map <std::string, std::string> t_MIMETypes;


class URI;
class Route;

class Config
{
	private:
		JsonData			_data;
		t_MIMETypes			_mimeTypes;

	public:
							Config();
							Config(const JsonData & Data);
		Config &			operator=(const Config & src);
							~Config();
		const char*			host() const;
		int					port() const;
		int					check();
		Route				getRoute(const URI & uri);
		void				parseMimeTypes(std::string mimeFilePath);
		std::string			getMimeType(const std::string extension);
		std::string			getErrorPage(int code, bool forceDefault) const;
		long				getMaxBodySize() const;
		const t_strArray&	getServerNames() const;
		bool				isSessionEnabled() const;
		int 				getSessionTimeout() const;
};

#endif