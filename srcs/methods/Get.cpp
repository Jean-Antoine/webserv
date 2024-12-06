/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:17:48 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/06 17:46:42 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Get.hpp"

Get::Get(Config *config,  Request & request):
	AMethod::AMethod(config, request)
{
}

// bool isFile(const std::string& path)
// {
// 	struct stat pathStat;
// 	if (stat(path.c_str(), &pathStat) != 0) {
// 		return false;
// 	}
// 	return S_ISREG(pathStat.st_mode);
// }

// bool isDirectory(std::string path)
// {
// 	struct stat pathStat;
// 	if (stat(path.c_str(), &pathStat) != 0) {
// 		return false;
// 	}
// 	return S_ISDIR(pathStat.st_mode);
// }

int Get::getFile(std::string &path)
{
	std::ifstream is(path.c_str(), std::ios::in | std::ios::binary);
	if (!is.is_open())
		return setResponseCode(404, "Not found");

	is.seekg(0, std::ios::end);
	std::streamsize fileSize = is.tellg();
	is.seekg(0, std::ios::beg);
	if (fileSize <= 0)
		return setResponseCode(204, "No Content");

	std::string content(fileSize, '\0'); // Allouer la chaîne
	if (!is.read(&content[0], fileSize)) // Lire dans la chaîne
	{
		is.close();
		return setResponseCode(500, "Internal Server Error");
	}
	_response.body = content;
	is.close();
	return true;
}

#include <dirent.h>

int Get::generateListingHTML(t_str_vec &items, std::string &dirPath)
{
	std::ostringstream html;

	html << "<html>\n<head><title>Index of " << dirPath << "</title></head>\n";
	html << "<body>\n<h1>Index of " << dirPath << "</h1>\n<ul>\n";

	// Lien vers le répertoire parent
	if (dirPath != _route.getRoot()) {
		html << "<li><a href=\"../\">Parent Directory</a></li>\n";
	}

	for (size_t i = 0; i < items.size(); ++i) 
	{
		const std::string& item = items[i];
		html << "<li><a href=\"" << item << "\">" << item << "</a></li>\n";
	}
	html << "</ul>\n</body>\n</html>";

	_response.body = html.str();
	_response.headers["Content-Type"] = "text/html; charset=UTF-8";
	return true;
}


int Get::generateDirectoryListing(std::string &path)
{
	t_str_vec	items;
	
	DIR	*dir = opendir(path.c_str()); //possible que pas les droits ?
	if (!dir)
		return setResponseCode(500, "Internal Server Error");
	
	struct dirent	*entry;
	while ((entry = readdir(dir)) != NULL)
	{
		std::string name = entry->d_name;
		if (name == "." || name == "..")
			continue;
		items.push_back(name);
	}
	if (closedir(dir))
		return setResponseCode(500, "Internal Server Error");
	setResponseCode(200, "ok");
	return generateListingHTML(items, path);
	
}


int Get::getFromDirectory(std::string &path)
{
	std::string	indexPath = path + "index.html";
	if (getRessource(indexPath))
		return true;
	if (true || _route.isDirectoryListingEnabled() == true) // test
		return generateDirectoryListing(path);
	return setResponseCode(403, "Forbidden");
}


int Get::getRessource(std::string &path)
{
	struct stat pathStat;
	if (!stat(path.c_str(), &pathStat))
	{
		if (S_ISREG(pathStat.st_mode))
			return getFile(path);
		else if (S_ISDIR(pathStat.st_mode))
			return getFromDirectory(path);
	}
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
