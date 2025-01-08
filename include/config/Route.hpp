/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:27:20 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/06 17:22:25 by jeada-si         ###   ########.fr       */
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
};

#endif