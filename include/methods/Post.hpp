/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:37:04 by lpaquatt          #+#    #+#             */
/*   Updated: 2025/01/20 17:56:06 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_HPP
# define POST_HPP

# define MAX_UPLOAD_FILE_SIZE (10 * 1024 * 1024) // 10 Mo
# define MAX_BOUNDARY_SIZE 70
# include "AMethod.hpp"

class Post : public AMethod
{
	private:
		Message			_content;
		std::string		_contentType;
		std::string		_boundary;
		int				parseBoundary();
		size_t			countBoundaries(t_lines &lines);
		bool			validateContent();
		bool			validateUploads();
		void			changeRessourcePath();
		int				parseContent();
		void			uploadFile();
		void			handleNewRessource();
		void			handleExistingRessource();
		void 			handleUploads();

	public:
						Post(Config &config,  Request & request);
						~Post() {};
		int				setResponse();
};

#endif