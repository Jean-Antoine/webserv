/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:17:48 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/12 09:26:10 by jeada-si         ###   ########.fr       */
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
	if (dirPath != _route.getRoot() + "/") // todo: plus propre quand uri faite ..?
		html << "<li><a href=\"../\">Parent Directory</a></li>\n";

	for (size_t i = 0; i < items.size(); ++i) 
	{
		const std::string& item = items[i];
		std::string path = _request.getPath() + item;
		if (getPathType(dirPath + item) == DIR_PATH) // todo: a checker avec uri
			path.append("/");
		html << "<li><a href=\"" << path << "\">" << item << "</a></li>\n";
	}
	html << "</ul>\n</body>\n</html>";

	_response.body = html.str();
	_response.headers["Content-Type"] = "text/html; charset=UTF-8";
	return true;
}

int Get::generateDirectoryListing(std::string &path)
{
	t_strVec	items;
	
	if (getDirectoryListing(path, items) == EXIT_FAILURE)
		return setResponseCode(500, "Internal Server Error");
	setResponseCode(200, "ok");
	return generateListingHTML(items, path);
	
}

int Get::getFromDirectory(std::string &path)
{
	if (access(path.c_str(), R_OK))
		return setResponseCode(403, "Forbidden");
	std::string	indexPath = path + "index.html"; //autres types de fichiers
	if (getPathType(indexPath) == FILE_PATH)
		return getFile(indexPath);
	if (true || _route.isDirListEnabled() == true) // test
		return generateDirectoryListing(path);
	return setResponseCode(403, "Forbidden");
}

int Get::getFile(std::string &path)
{
	if (access(path.c_str(), R_OK))
		return setResponseCode(503, "Forbidden");

	if (readFile(path, _response.body) == EXIT_FAILURE)
		return setResponseCode(500, "Internal Server Error");
	
	if (_response.body.empty())
		return setResponseCode(204, "No Content");

	return true;
}

int Get::getRessource(std::string &path)
{
	t_pathType	type = getPathType(path);
	if (type == FILE_PATH)
		return getFile(path);
	else if (type == DIR_PATH)
		return getFromDirectory(path);
	return setResponseCode(404, "Not found" );
}

std::string Get::getResponse()
{
	if (!isValid())
		return errorResponse();
	std::string path = _route.getLocalPath();
	if (!getRessource(path))
		return errorResponse();
	return buildResponse();
}
