/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ressource.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:30:29 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/08 16:00:50 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ressource.hpp"
#include <iostream>

Ressource::Ressource()
{
}

Ressource::Ressource(Path root, Path relativePath, std::string defaultFile):
	_relativePath(relativePath)
{
	_path = root + _relativePath;
	if (_path.exist() && !_path.getStats() && _path.isDir())
		_path = _path + defaultFile;
	Logs(BLUE) < "Ressource: "
		< _path.litteral() < "\n";
}

Ressource::Ressource(const Ressource &src)
{
	*this = src;
}

Ressource& Ressource::operator=(const Ressource &src)
{
	_relativePath = src._relativePath;
	_path = src._path;
	return *this;
}

Ressource::~Ressource()
{
}

bool	Ressource::isCgi() const
{
	std::string	ext = _path.extension();
	
	if (ext == "php" || ext == "py")
		return true;
	return false;
}

bool	Ressource::forbidden()
{
	return _relativePath.throughParent();
}

std::string	Ressource::getExtension() const
{
	return _path.extension();
}

const Path &	Ressource::getPath() const
{
	return _path;
}

const Path &	Ressource::getRelativePath() const
{
	return _relativePath;
}

int	Ressource::readFile()
{
	std::ifstream		fs;
	std::ostringstream	ss;
	
	fs.open(_path.litteral().c_str(), std::fstream::in);
	if (fs.fail() || !fs.is_open())
		return EXIT_FAILURE;
	ss << fs.rdbuf();
	_fileContent = ss.str();
	fs.close();
	return fs.fail() || ss.fail();
}

const std::string &	Ressource::fileContent() const
{
	return _fileContent;
}

int	Ressource::readDir()
{
	DIR				*dir = opendir(_path.litteral().c_str());
	struct dirent	*entry;
	
	if (!dir)
		return EXIT_FAILURE;	
	while ((entry = readdir(dir)) != NULL)
	{
		std::string name = entry->d_name;
		if (name == "." || name == "..")
			continue ;
		_dirContent.push_back(_path + name);
	}
	if (closedir(dir))
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

const std::vector < Path > &	Ressource::dirContent() const
{
	return _dirContent;
}