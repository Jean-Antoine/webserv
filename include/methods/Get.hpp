/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:22:59 by lpaquatt          #+#    #+#             */
/*   Updated: 2025/01/17 12:26:51 by lpaquatt         ###   ########.fr       */
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
		int				setResponse();

	private:
		void			setResponseFile();
		void			setResponseDir();
		void			setRedirection();
};


#endif
