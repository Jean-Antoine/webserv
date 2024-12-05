/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invalid.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:40:53 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/05 15:23:01 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INVALID_HPP
# define INVALID_HPP
# include "AMethod.hpp"

class Invalid : public AMethod
{
	private:
		
	public:
		Invalid(Config *config,  Request & request);
		~Invalid() {};		
		std::string	getResponse();
};

#endif