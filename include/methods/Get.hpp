/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:22:59 by lpaquatt          #+#    #+#             */
/*   Updated: 2025/01/16 09:01:17 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_HPP
# define GET_HPP

# include <unistd.h>
# include "AMethod.hpp"

class Get : public AMethod
{
	public:
					Get(Config &config,  Request & request);
					~Get();		
		std::string	getResponse();
	private:
		void		setResponseFile();
		void		setResponseDir();
		void		setRedirection();
};


#endif
