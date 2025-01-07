/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ressource.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:18:53 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/06 18:35:09 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RESSOURCE_HPP__
# define __RESSOURCE_HPP__
# include <string>
# include <iostream>
# include <iomanip>
# include "Route.hpp"

class Ressource
{
	private:
		Path				_relativePath;
		Path				_path;
	public:
							Ressource();
							Ressource(Route & route, Path uriPath);
							Ressource(const Ressource &src);
							~Ressource();
		Ressource & 		operator=(const Ressource &src);
		bool				forbidden();
		std::string			getPath() const;
		bool				isCgi() const;
		std::string			getExtension() const;
};

#endif