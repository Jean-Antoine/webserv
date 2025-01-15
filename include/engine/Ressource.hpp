/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ressource.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:18:53 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/15 10:41:13 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RESSOURCE_HPP__
# define __RESSOURCE_HPP__
# include <string>
# include <iostream>
# include <iomanip>
# include <fstream>
# include <sstream>
# include <vector>
# include "Path.hpp"
# include <dirent.h>

class Ressource
{
	private:
		Path								_relativePath;
		Path								_path;
		std::string							_fileContent;
		std::vector < Path >				_dirContent;
	public:
											Ressource();
											Ressource(Path path);
											Ressource(Path root, Path relativePath, std::string defaultFile);
											Ressource(const Ressource &src);
											~Ressource();
		Ressource & 						operator=(const Ressource &src);
		bool								forbidden();
		const Path &						getPath() const;
		const Path &						getRelativePath() const;
		bool								isCgi() const;
		std::string							getExtension() const;
		int									readFile();
		const std::string &					fileContent() const;
		int									readDir();
		const std::vector < Path > &		dirContent() const;
};

#endif