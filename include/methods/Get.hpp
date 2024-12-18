/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:22:59 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/17 16:35:22 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_HPP
# define GET_HPP

# include <unistd.h>
# include "AMethod.hpp"
# include "path.hpp"

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
		int			getCgi();
		int			getFromDirectory(std::string &path);
		int			generateDirectoryListing(std::string &path);
		int			generateListingHTML(t_strVec &items, std::string &dirPath);

};


#endif
