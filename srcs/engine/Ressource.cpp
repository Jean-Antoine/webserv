/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ressource.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:30:29 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/16 13:49:43 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ressource.hpp"
#include <iostream>

Ressource::Ressource()
{
}

Ressource::Ressource(Path path):
	_path(path)
{
	_path.getStats();
}

Ressource::Ressource(Path root, Path relativePath, std::string defaultFile):
	_relativePath(relativePath)
{
	_path = root + _relativePath;
	if (_path.exist() && !_path.getStats() && _path.isDir() && _path.readable())
	{
		Path index = _path + defaultFile;
		
		if (index.exist())
			_path = index;
	}
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

void Ressource::setPath(Path path)
{
	_path = path;
}

int	Ressource::readFile()
{
	std::ifstream		fs;
	std::ostringstream	ss;
	
	fs.open(_path.litteral().c_str(), std::fstream::in);
	if (fs.fail() || !fs.is_open())
		return EXIT_FAILURE;
	if (fs.rdbuf()->in_avail() == 0)
		return EXIT_SUCCESS;
	ss << fs.rdbuf();
	_fileContent = ss.str();
	fs.close();
	return fs.fail() || ss.fail();
}

int Ressource::writeFile(std::string content)
{
	std::string filePath = _path.litteral();
	Path dir = _path.getParent();
	if (!dir.exist())
	{
		if (mkdir(dir.litteral().c_str(), 0755))
			return EXIT_FAILURE;
	}
	else if (!dir.isDir())
		return EXIT_FAILURE;
	
	std::ofstream fs(filePath.c_str());
	if (!fs.is_open())
		return EXIT_FAILURE;
	fs << content;
	if (fs.fail()){
		fs.close();
		return EXIT_FAILURE;
	}
	fs.close();
	return EXIT_SUCCESS;
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