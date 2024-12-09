/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 17:12:27 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/10 14:53:22 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "path.hpp"

t_pathType getPathType(std::string path)
{
	struct stat pathStat;
	if (!stat(path.c_str(), &pathStat))
	{
		if (S_ISREG(pathStat.st_mode))
			return FILE_PATH;
		else if (S_ISDIR(pathStat.st_mode))
			return DIR_PATH;
	}
	return OTHER_PATH;
}

int	getDirectoryListing (std::string dirPath, t_strVec &items)
{
	DIR	*dir = opendir(dirPath.c_str()); //possible que pas les droits ?
	if (!dir)
		return EXIT_FAILURE;
	
	struct dirent	*entry;
	while ((entry = readdir(dir)) != NULL)
	{
		std::string name = entry->d_name;
		if (name == "." || name == "..")
			continue;
		items.push_back(name);
	}
	if (closedir(dir))
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

int	readFile(std::string path, std::string &dest)
{
	std::ifstream	file(path.c_str(), std::ios::in);
	if (!file.is_open())
		return EXIT_FAILURE;

	std::ostringstream content;
	content << file.rdbuf();
	if (file.fail())
		return EXIT_FAILURE;
	dest = content.str();
	return EXIT_SUCCESS;
}
