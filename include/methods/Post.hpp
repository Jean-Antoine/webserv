/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:37:04 by lpaquatt          #+#    #+#             */
/*   Updated: 2025/01/14 18:45:22 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_HPP
# define POST_HPP

# include "AMethod.hpp"


class Post : public AMethod
{
	private:
		Message		_content;
		std::string	_contentType;
		std::string	_boundary;
		
	public:
		Post(Config *config,  Request & request);
		~Post() {};
		std::string	getResponse();
		
		void		uploadFile();
		int			parseBoundary();
		size_t		countBoundaries(t_lines &lines);
		bool		isValidContent();
		int			parseContent();
		void		handleNewRessource();

};


#endif