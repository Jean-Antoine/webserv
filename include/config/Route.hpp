/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:27:20 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/13 15:47:11 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ROUTE_HPP__
# define __ROUTE_HPP__
# include "utils.hpp"
# include "JsonData.hpp"
# include "URI.hpp"
# include "Path.hpp"

class Route
{
	private:
		JsonData			*_data;
	public:
							Route();
							Route(JsonData *data);
							Route(const Route &src);
							~Route();
		Route & 			operator=(const Route &src);
		bool				empty() const;
		const std::string &	getPath() const;
		const std::string &	getRoot() const;
		const std::string &	getDefaultFile() const;
		bool				isDirListEnabled() const;
		const t_strVec &	getAllowedMethods() const;
		bool				isMethodAllowed(std::string method) const;
		bool				isCgiEnabled(const char *ext) const;
		const std::string &	getCgiBinPath(const char *ext) const;
		bool				isRedirectionEnabled() const;
		const std::string &	getRedirection() const;
		bool				isUploadsEnabled() const;
		const std::string &	getUploads() const;
};

#endif