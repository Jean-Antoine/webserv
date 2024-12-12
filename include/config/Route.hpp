/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:27:20 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/12 18:21:39 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ROUTE_HPP__
# define __ROUTE_HPP__
# include "utils.hpp"
# include "JsonData.hpp"
# include "URI.hpp"

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
		bool				isMethodAllowed(std::string method) const;
		const t_strVec &	getAllowedMethods() const;
		bool				isCGIEnabled() const;
		const t_strVec &	getCGIExtensions() const;
		bool				isCGIExtension(std::string extension);
};

#endif