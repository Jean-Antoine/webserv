/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:48:27 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/19 03:13:39 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Delete.hpp"

Delete::Delete(Config &config, Request &request):
	AMethod::AMethod(config, request)
{
}

Delete::~Delete()
{
}

void Delete::setResponseFile()
{
	if (!_ressource.getPath().writable())
		_response.setResponseCode(403, 
			"file " + _ressource.getPath().litteral() + " is not writable");
	else if (_ressource.remove())
		_response.setResponseCode(500, "failed to delete file " + _ressource.getPath().litteral());
	else
		_response.setResponseCode(204, "file " + _ressource.getPath().litteral() + " deleted");
}

bool Delete::validateDir()
{
	if (!_ressource.getPath().readable() || _ressource.readDir()){
		_response.setResponseCode(403, "can't read directory "
			+ _ressource.getPath().litteral());
		return false;
	}
	if (!_ressource.isDirEmpty()){
		_response.setResponseCode(403, "can't delete non-empty directory "
			+ _ressource.getPath().litteral());
		return false;
	}
	return true;
}

int	Delete::setResponse()
{
	if (!_ressource.getPath().exist())
		_response.setResponseCode(404, _ressource.getPath().litteral() + " does not exist");
	else if (!_ressource.getPath().writable())
		_response.setResponseCode(403, 
			"wrong permissions to delete " + _ressource.getPath().litteral());
	else if (_ressource.getPath().isDir() && !validateDir())
		{}
	else if (_ressource.remove())
		_response.setResponseCode(500, "failed to delete resource " 
			+ _ressource.getPath().litteral());
	else
		_response.setResponseCode(204, _ressource.getPath().litteral() + " deleted successfully");
	return EXIT_SUCCESS;
}