/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ressource.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:30:29 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/07 10:16:39 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ressource.hpp"
#include <iostream>

Ressource::Ressource()
{
}

Ressource::Ressource(Route & route, Path uriPath):
	_relativePath(uriPath),
	_path(uriPath)
{
	if (route.empty())
		return ;
	_relativePath.trim(route.getPath());
	_path = Path(route.getRoot()) + _relativePath;
	if (_relativePath.size() == 0)
		_path = route.getRoot() + route.getDefaultFile();
	Logs(BLUE) < "Ressource: "
		< getPath() < "\n";
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
	return _relativePath.throughParent() || !_path.readable();
}

std::string	Ressource::getExtension() const
{
	return _path.extension();
}

std::string	Ressource::getPath() const
{
	return _path.litteral();
}
