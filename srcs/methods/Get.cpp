/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:17:48 by lpaquatt          #+#    #+#             */
/*   Updated: 2025/01/08 11:27:09 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Get.hpp"

Get::Get(Config *config,  Request & request):
	AMethod::AMethod(config, request)
{
}

Get::~Get()
{
}

static std::string	dirListingHtmlHead(std::string dirName)
{
	return	"<!DOCTYPE html> \
			<html lang=\"en\"> \
			<head> \
				<meta charset=\"UTF-8\"> \
				<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> \
				<title>Directory Listing</title> \
				<style> \
					body { \
						font-family: Arial, sans-serif; \
						margin: 20px; \
					} \
					table { \
						width: 100%; \
						border-collapse: collapse; \
					} \
					th, td { \
						border: 1px solid #ddd; \
						padding: 8px; \
						text-align: left; \
					} \
					th { \
						background-color: #f4f4f4; \
					} \
					a { \
						text-decoration: none; \
						color: #007BFF; \
					} \
					a:hover { \
						text-decoration: underline; \
					} \
				</style> \
			</head> \
			<body> \
				<h1>Directory Listing for " + dirName + "</h1> \
				<table> \
					<thead> \
						<tr> \
							<th>Name</th> \
							<th>Type</th> \
							<th>Size</th> \
							<th>Last Modified</th> \
						</tr> \
					</thead> \
					<tbody>";
}


static std::string	dirListingHtmlRow(
	std::string href,
	std::string name,
	std::string type,
	std::string size,
	std::string mDate)
{
	return	"<tr> \
				<td> \
					<a href=\"" + href + "\"> \
						" + name + " \
					</a> \
				</td> \
				<td> \
					" + type + " \
				</td> \
				<td> \
					" + size + " \
				</td> \
				<td> \
					" + mDate + " \
				</td> \
			</tr>";
}

static std::string	dirListingHtmlTail()
{
	return "</tbody></table></body></html>";
}

static std::string	dirListingHtml(const std::vector < Path > & content, const Path & dirPath)
{
	std::string	output = dirListingHtmlHead(dirPath.litteral())
		+dirListingHtmlRow((dirPath + "..").litteral(), "..", "-", "-", "-");
	
	for (std::vector< Path >::const_iterator item = content.begin();
		item != content.end(); item++)
	{
		Path	href = dirPath + item->basename();
		
		output += dirListingHtmlRow(
			href.litteral(),
			item->basename(),
			item->isDir() ? "Directory" : "File",
			to_string(item->fileSize()) + "bytes",
			item->fileLastModifiedStr()
		);
	}
	output += dirListingHtmlTail();
	return output;
}

void Get::setResponseDir()
{
	
	if (!_route.isDirListEnabled())
		return _response.setResponseCode(403, "no index file in directory "
			+ _ressource.getPath().litteral());
	if (!_ressource.getPath().readable() || _ressource.readDir())
		return _response.setResponseCode(403, "can't read directory "
			+ _ressource.getPath().litteral());
	_response.setBody(dirListingHtml(_ressource.dirContent(), _ressource.getRelativePath()));
	_response.setHeader("Content-Type", "text/html; charset=utf-8");
}

void Get::setResponseFile()
{
	if (!_ressource.getPath().readable())
		return _response.setResponseCode(403, "can't read file "
			+ _ressource.getPath().litteral());
	if (_ressource.readFile())
		return _response.setResponseCode(500, "failed to read file "
			+ _ressource.getPath().litteral());
	_response.setBody(_ressource.fileContent());
	_response.setHeader("Content-Type", getMimeType());
}

std::string Get::getResponse()
{
	if (!isValid() || _ressource.isCgi())
		return _response.getResponse();
	if (_ressource.getPath().isDir())
		setResponseDir();
	else if (_ressource.getPath().isFile())
		setResponseFile();
	else
		_response.setResponseCode(500, "Unknown type of file");
	return _response.getResponse();
}

//todo @leon ?? Ajoutez une gestion des en-têtes If-Modified-Since et If-None-Match pour renvoyer une réponse 304 (Not Modified) lorsque le contenu n'a pas changé.