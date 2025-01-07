/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Path.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 13:40:23 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/07 09:38:25 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PATH_HPP__
# define __PATH_HPP__
# include <string>
# include <vector>
# include "utils.hpp"
# include <sys/stat.h>
# include "unistd.h"

class Path
{
	private:
		t_strVec					_path;
		const t_strVec&				get() const;
		void						normalise();
		void						insert(t_strVec::const_iterator begin,
											t_strVec::const_iterator end);
	public:
									Path();
									Path(std::string path);
									Path(const Path &src);
									~Path();
		Path &						operator=(const Path &src);
		Path &						operator=(const std::string &src);
		Path						operator+(const Path &obj);
		const std::string &			operator[](size_t i) const;
		bool						in(const Path &obj) const;
		size_t						size() const;
		size_t						compare(const Path &obj);
		std::string					basename() const;
		std::string					extension() const;
		std::string					litteral() const;
		void						trim(const Path & root);
		bool						isDir() const;
		bool						readable() const;
		bool						throughParent() const;
};

std::ostream&	operator<<(std::ostream & out, const Path & path);

#endif