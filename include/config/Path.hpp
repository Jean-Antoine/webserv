/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Path.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 13:40:23 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/18 19:47:03 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PATH_HPP__
# define __PATH_HPP__
# include <string>
# include <vector>
# include <sys/stat.h>
# include <fstream>
# include <sstream>
# include "utils.hpp"

class Path
{
	private:
		t_strVec					_path;
		struct stat					_stats;
		const t_strVec&				get() const;
		void						normalise();
		void						insert(t_strVec::const_iterator begin,
											t_strVec::const_iterator end);		
	public:
									Path();
									Path(const std::string path);
									Path(const char *path);
									Path(const Path &src);
									~Path();
		Path &						operator=(const Path &src);
		Path &						operator=(const std::string &src);
		Path						operator+(const Path &obj) const;
		const std::string &			operator[](size_t i) const;
		bool						in(const Path &obj) const;
		size_t						size() const;
		size_t						compare(const Path &obj);
		std::string					basename() const;
		std::string					extension() const;
		std::string					litteral() const;
		void						trim(const Path & root);
		int							getStats();
		bool						isDir() const;
		bool						isFile() const;
		off_t						fileSize() const;
		bool						exist() const;
		timespec					fileLastModified() const;
		std::string					fileLastModifiedStr() const;
		bool						readable() const;
		bool						writable() const;
		bool						throughParent() const;
		Path						getParent() const;
};

std::ostream&	operator<<(std::ostream & out, const Path & path);

#endif