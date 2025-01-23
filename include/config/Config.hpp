/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:27:11 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/22 23:25:03 by lpaquatt         ###   ########.fr       */
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
typedef std::map < std::string, int >		t_sessions;

class URI;
class Route;

class Config
{
	private:
		JsonData			_data;
		t_MIMETypes			_mimeTypes;
		t_sessions			_sessions;

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
		void				incrementSessionReqCnt(const std::string & id);
		int 				getSessionReqCnt(const std::string & id);
};

#endif