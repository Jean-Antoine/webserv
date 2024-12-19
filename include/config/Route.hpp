/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:27:20 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/19 11:08:13 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ROUTE_HPP__
# define __ROUTE_HPP__
# include "utils.hpp"
# include "JsonData.hpp"
# include "URI.hpp"
# include "path.hpp"

class Route
{
	private:
		JsonData			_data;
		std::string			_localPath;
	public:
							Route();
							Route(JsonData & data, t_str uriPath);
							Route(const Route &src);
							~Route();
		Route & 			operator=(const Route &src);
		bool				bad();
		const std::string &	getRoot() const;
		const std::string &	getLocalPath() const;
		const std::string	getDefaultFile() const;
		bool				isDirListEnabled() const;
		const t_strVec &	getAllowedMethods() const;
		bool				isMethodAllowed(std::string method) const;
		bool				isCgi() const;
		bool				isCgiEnabled() const;
		const std::string &	getCgiBinPath() const;
};

#endif