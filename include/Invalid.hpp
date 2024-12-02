/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invalid.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:40:53 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/02 12:59:19 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INVALID_HPP
# define INVALID_HPP

# include "AMethod.hpp"

class Invalid : public AMethod
{
	private:
		
	public:
		Invalid() {};
		~Invalid() {};
		
		std::string	response();
};

#endif