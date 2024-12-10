/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:27:11 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/10 23:37:20 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__
# include "JsonParser.hpp"
# include "URI.hpp"
# include "Route.hpp"

//pour MIME types
# include <map>
# include <fstream>
# include <sstream>
# include <string>

# define MIME_TYPES_FILE "/etc/mime.types"
typedef std::map<std::string, std::string> t_MIMETypes;

class URI;
class Config
{
	private:
		JsonData		_data;
		t_MIMETypes		_mimeTypes;

	public:
						Config();
						Config(const JsonData& Data);
		Config &		operator=(const Config & src);
						~Config();
		const char*		host() const;
		int				port() const;
		int				check();
		t_strVec		getRoutes() const;
		Route			getRoute(URI & uri);

		void			parseMimeTypes(std::string mimeFilePath);
		std::string		getMimeType(const std::string& extension);

};

#endif