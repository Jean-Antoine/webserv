/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:22:59 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/06 17:16:18 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_HPP
# define GET_HPP

# include <sys/stat.h>

# include "AMethod.hpp"


class Get : public AMethod
{
	private:
		// std::string	_path;
		
	public:
		Get(Config *config,  Request & request);
		~Get() {};		
		std::string	getResponse();
		int			getRessource(std::string &path);
		int			getFile(std::string &path);
		int			getFromDirectory(std::string &path);
		int			generateDirectoryListing(std::string &path);
		int			generateListingHTML(t_str_vec &items, std::string &dirPath);

};


#endif
