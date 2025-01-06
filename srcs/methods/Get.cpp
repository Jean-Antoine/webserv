/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:17:48 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/20 18:28:23 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Get.hpp"

Get::Get(Config *config,  Request & request):
	AMethod::AMethod(config, request)
{
}

//todo a clean plus tard faire un truc plus joli
int Get::generateListingHTML(t_strVec &items, std::string &dirPath)
{
	std::ostringstream html;

	html << "<html>\n<head><title>Index of " << _request.getURI().getPath() << "</title></head>\n";
	html << "<body>\n<h1>Index of " << _request.getURI().getPath() << "</h1>\n<ul>\n";

	// Lien vers le répertoire parent
	if (dirPath != _route.getRoot() + "/") // todo: plus propre quand uri faite ..? //isRootDirectory
		html << "<li><a href=\"../\">Parent Directory</a></li>\n"; //todo: a corriger, c'est faux dans des sous repertoires j'ai l'impression

	for (size_t i = 0; i < items.size(); ++i) 
	{
		const std::string & item = items[i];
		// std::string path = _request.getPath() + item;
		std::string path = concatPath(_request.getURI().getPath(), item);
		if (getPathType(dirPath + item) == DIR_PATH) // todo: a checker avec uri
			path.append("/");
		html << "<li><a href=\"" << path << "\">" << item << "</a></li>\n";
	}
	html << "</ul>\n</body>\n</html>";

	_response.setBody(html.str());
	_response.setHeader("Content-Type", "text/html; charset=UTF-8");
	return true;
}

int Get::generateDirectoryListing(std::string &path)
{
	t_strVec	items;
	
	if (getDirectoryListing(path, items) == EXIT_FAILURE)
		return _response.setResponseCode(500, "failed to read directory " + path); //todo: @leon mettre des relative paths
	_response.setResponseCode(200, "ok");
	return generateListingHTML(items, path);
	
}

int Get::getFromDirectory(std::string &path)
{
	if (access(path.c_str(), R_OK))
		return _response.setResponseCode(403, "can't read directory " + path);

	std::string	indexPath = path + _route.getDefaultFile();
	if (getPathType(indexPath) == FILE_PATH)
		return getFile(indexPath);

	if (_route.isDirListEnabled())
		return generateDirectoryListing(path);

	return _response.setResponseCode(403, "no index file in directory " + path);
}

int Get::getFile(std::string &path)
{
	std::string body;

	if (access(path.c_str(), R_OK))
		return _response.setResponseCode(403, "can't read file " + path); 
	if (readFile(path, body) == EXIT_FAILURE)
		return _response.setResponseCode(500, "failed to read file "+ path); 
	if (body.empty())
		return _response.setResponseCode(204, path + " is empty");
	_response.setBody(body);
	_response.setHeader("Content-Type", getMimeType(path));
	return true;
}

int Get::getRessource(std::string &path)
{
	t_pathType	type = getPathType(path);

	switch (type)
	{
	case FILE_PATH:
		return getFile(path);
	case DIR_PATH:
		return getFromDirectory(path);
	default:
		return _response.setResponseCode(404, path + " not found");
	}
}

std::string Get::getResponse()
{
	if (!isValid() || _route.isCgi())
		return _response.getResponse();
	std::string path = _route.getLocalPath();
	getRessource(path);
	return _response.getResponse();
}

//todo @leon ?? Ajoutez une gestion des en-têtes If-Modified-Since et If-None-Match pour renvoyer une réponse 304 (Not Modified) lorsque le contenu n'a pas changé.