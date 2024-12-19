/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:17:48 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/19 10:25:52 by jeada-si         ###   ########.fr       */
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

	html << "<html>\n<head><title>Index of " << dirPath << "</title></head>\n";
	html << "<body>\n<h1>Index of " << dirPath << "</h1>\n<ul>\n";

	// Lien vers le répertoire parent
	// if (dirPath != _route.getRoot() + "/") // todo: plus propre quand uri faite ..? //isRootDirectory
		html << "<li><a href=\"./..\">Parent Directory</a></li>\n";

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
		return _response.setResponseCode(500, "Internal Server Error");
	_response.setResponseCode(200, "ok");
	return generateListingHTML(items, path);
	
}

int Get::getFromDirectory(std::string &path)
{
	if (access(path.c_str(), R_OK))
		return _response.setResponseCode(403, "Forbidden");

	std::string	indexPath = path + _route.getDefaultFile();
	if (getPathType(indexPath) == FILE_PATH)
		return getFile(indexPath);
	if (true || _route.isDirListEnabled() == true) // test
		return generateDirectoryListing(path);

	return _response.setResponseCode(403, "Forbidden");
}

int Get::getFile(std::string &path)
{
	if (access(path.c_str(), R_OK))
		return _response.setResponseCode(403, "Forbidden");

	std::string	body;
	if (readFile(path, body) == EXIT_FAILURE)
		return _response.setResponseCode(500, "Internal Server Error");
	
	if (body.empty())
		return _response.setResponseCode(204, "No Content");

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
		return _response.setResponseCode(404, "Not found" );
	}
}

std::string Get::getResponse()
{
	if (!isValid() || _route.isCgi())
		return _response.getResponse();
	std::string path = _route.getLocalPath();
	if (getPathType(path) == DIR_PATH) // todo: pour test, a degager quand bonne uri
		path.append("/");
	getRessource(path);
	return _response.getResponse();
}
