/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:27:20 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/09 14:03:10 by lpaquatt         ###   ########.fr       */
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
		JsonData &		_data;
	public:
						Route(JsonData & data);
						Route(const Route &src);
						~Route();
		Route & 		operator=(const Route &src);
		int				isMethodAllowed(t_method method) const;
		t_str_vec		&getAllowedMethods() const;
		std::string		getLocalPath(std::string ressourcePath);
		std::string		getRoot();
		bool			isDirectoryListingEnabled();
};

#endif