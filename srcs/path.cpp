/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 17:12:27 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/13 17:01:45 by lpaquatt         ###   ########.fr       */
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
	DIR	*dir = opendir(dirPath.c_str());
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

int	readFile(std::string path, std::string & dest)
{
	std::ifstream	file(path.c_str(), std::ios::in | std::ios::binary); //todo : checker quel option mettre..?
	if (!file.is_open())
		return EXIT_FAILURE;

	std::ostringstream content;
	content << file.rdbuf();
	if (file.fail())
		return EXIT_FAILURE;
	dest = content.str();
	return EXIT_SUCCESS;
}

std::string	concatPath(const std::string & prefix, const std::string & path)
{
	std::string	out = prefix;

	if (*out.rbegin() != '/')
		out.append("/");
	if (path[0] == '/')
		out.append(path.begin() + 1, path.end());
	else
		out.append(path);
	return out;
}

std::string	getExtension(const std::string & path)
{
	std::string	extension;
	size_t dotPos = path.find_last_of('.');
	
	if (dotPos != std::string::npos)
		extension = path.substr(dotPos + 1);
	return extension;
}
