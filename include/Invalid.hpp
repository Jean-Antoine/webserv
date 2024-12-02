/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invalid.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:40:53 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/02 19:35:47 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INVALID_HPP
# define INVALID_HPP

# include "AMethod.hpp"

class Invalid : public AMethod
{
	private:
		
	public:
		Invalid(Config &config, Request &request): AMethod(config, request) {};
		~Invalid() {};
		
		std::string	getResponse();
};

#endif