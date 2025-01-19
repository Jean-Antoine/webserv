/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:47:34 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/19 02:45:03 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __DELETE_HPP__
# define __DELETE_HPP__
# include "AMethod.hpp"

class Config;
class Request;

class Delete: public AMethod
{
	private:
		
	public:
				Delete(Config &config, Request & request);
				~Delete();
			
		int		setResponse();
		bool	validateDir();
		void	setResponseFile();
};

#endif