/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:17:48 by lpaquatt          #+#    #+#             */
/*   Updated: 2025/01/19 01:36:54 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Get.hpp"

Get::Get(Config &config,  Request & request):
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
				<title>" + dirName + "</title> \
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
						text-align: right; \
					} \
					.left { \
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
				<h1 style='margin-left:5%'>Directory Listing for " + dirName + "</h1> \
				<table> \
					<thead> \
						<tr> \
							<th class=left>Name</th> \
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
				<td class=left> \
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
			to_string(item->fileSize()) + " bytes",
			item->fileLastModifiedStr()
		);
	}
	output += dirListingHtmlTail();
	return output;
}

void	Get::setResponseDir()
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

void	Get::setResponseFile()
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

void	Get::setRedirection()
{
	_response.setResponseCode(302, "Redirection to " + _route.getRedirection());
	_response.setHeader("Location", _route.getRedirection());
}

int	Get::setResponse()
{
	// if ((!_request.getBody().empty() && _request.getBody() != "\r\n")
	// 	|| _request.isHeaderSet("Content-Length"))
	// 	_response.setResponseCode(400, "GET request should not have a body");
	if (_route.isRedirectionEnabled())
		setRedirection();
	else if (!_ressource.getPath().exist())
		_response.setResponseCode(404, _ressource.getPath().litteral() + "does not exist");
	else if (_ressource.isCgi())
		return executeCgi();
	else if (_ressource.getPath().isDir())
		setResponseDir();
	else if (_ressource.getPath().isFile())
		setResponseFile();
	else
		_response.setResponseCode(500, "unknown type of file");
	return EXIT_SUCCESS;
}

//todo @leon ?? Ajoutez une gestion des en-têtes If-Modified-Since et If-None-Match pour renvoyer une réponse 304 (Not Modified) lorsque le contenu n'a pas changé.