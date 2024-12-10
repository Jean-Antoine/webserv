/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:27:20 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/10 13:34:58 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ROUTE_HPP__
# define __ROUTE_HPP__
# include "utils.hpp"
# include "JsonData.hpp"

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
		bool			bad();
		int				isMethodAllowed(std::string method) const;
		t_strVec &		getAllowedMethods() const;
};

#endif