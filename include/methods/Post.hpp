/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:37:04 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/17 12:38:49 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_HPP
# define POST_HPP

# include "AMethod.hpp"


class Post : public AMethod
{
	private:
		
	public:
		Post(Config *config,  Request & request);
		~Post() {};		
		std::string	getResponse();
};


#endif