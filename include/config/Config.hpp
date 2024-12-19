/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:27:11 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/19 11:08:10 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__
# include "JsonParser.hpp"
# include "URI.hpp"
# include "Route.hpp"
# include <map>
# include <fstream>
# include <sstream>
# include <string>
# define MIME_TYPES_FILE "/etc/mime.types"

typedef std::map <std::string, std::string> t_MIMETypes;

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
		Route			getRoute(const URI & uri);
		void			parseMimeTypes(std::string mimeFilePath);
		std::string		getMimeType(const std::string & extension);

};
#endif