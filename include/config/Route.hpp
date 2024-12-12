/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:27:20 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/10 21:22:01 by lpaquatt         ###   ########.fr       */
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
		JsonData		_data;
	public:
						Route();
						Route(JsonData & data);
						Route(const Route &src);
						~Route();
		Route & 		operator=(const Route &src);
		std::string		getLocalPath(std::string ressourcePath);
		std::string		getRoot();
		std::string		getDefaultFile();
		bool			isDirectoryListingEnabled();
		bool			bad();
		int				isMethodAllowed(std::string method) const;
		t_strVec &		getAllowedMethods() const;
};

#endif