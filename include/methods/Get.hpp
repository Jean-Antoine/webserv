/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:22:59 by lpaquatt          #+#    #+#             */
/*   Updated: 2025/01/07 14:46:54 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_HPP
# define GET_HPP

# include <unistd.h>
# include "AMethod.hpp"
# include "path.hpp"

class Get : public AMethod
{
	public:
					Get(Config *config,  Request & request);
					~Get();		
		std::string	getResponse();
	private:
		int			setResponseFile();
		int			setResponseDir();
		int			getFromDirectory(std::string &path);
		int			generateDirectoryListing(std::string &path);
		int			generateListingHTML(t_strVec &items, std::string &dirPath);		
};


#endif
