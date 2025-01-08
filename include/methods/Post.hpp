/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:37:04 by lpaquatt          #+#    #+#             */
/*   Updated: 2025/01/06 15:51:05 by lpaquatt         ###   ########.fr       */
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
		int			postContent();
		int			postFormUrlEncoded();
		int			parseFormUrlEncoded(std::map<std::string, std::string> &parsedData);

};


#endif