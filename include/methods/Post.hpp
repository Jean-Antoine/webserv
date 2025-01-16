/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:37:04 by lpaquatt          #+#    #+#             */
/*   Updated: 2025/01/16 09:39:02 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_HPP
# define POST_HPP

# include "AMethod.hpp"


class Post : public AMethod
{
	private:
		Message			_content;
		std::string		_contentType;
		std::string		_boundary;
	public:
						Post(Config &config,  Request & request);
						~Post() {};
		int				setResponse();
		void			uploadFile();
		int				parseBoundary();
		size_t			countBoundaries(t_lines &lines);
		bool			isValidContent();
		int				parseContent();
		void			handleNewRessource();

};

#endif