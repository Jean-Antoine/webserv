/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:36:15 by lpaquatt          #+#    #+#             */
/*   Updated: 2025/01/20 14:06:30 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Post.hpp"

Post::Post(Config &config,  Request & request):
	AMethod::AMethod(config, request)
{
}

void Post::uploadFile()
{
	if (!_ressource.getPath().getParent().writable())
	{
		_response.setResponseCode(403, "wrong permissions to create "
			+ _ressource.getPath().litteral());
		return;
	}
	if (_ressource.writeFile(_content.getBody()))
		_response.setResponseCode(500, 
			"error while uploading file " + _ressource.getPath().litteral());
	else
		_response.setResponseCode(201, _ressource.getPath().litteral() + " uploaded successfully");
}

bool Post::validateContent()
{
	if (_content.getHeader("Content-Disposition").empty()
		|| _content.getHeader("Content-Type").empty()){
		_response.setResponseCode(400, "missing header in multipart/form-data part");
		return false;
	}
	if (_content.getHeader("Content-Disposition").find("filename=") == std::string::npos) {
		_response.setResponseCode(400, "missing filename in Content-Disposition header");
		return false;
	}
	if (getMimeType() != _content.getHeader("Content-Type") &&
		_content.getHeader("Content-Type") != "application/octet-stream"){
		_response.setResponseCode(400, "conflicting content types in upload of " + _ressource.getPath().litteral());
		return false;
	}
	if (_content.getBody().length() > MAX_UPLOAD_FILE_SIZE) 
	{
		_response.setResponseCode(413, "uploaded file exceeds the maximum allowed size");
		return false;
	}
	return true;
}

 

int Post::parseBoundary()
{
	size_t boundaryPos = _contentType.find("boundary=");
	if (boundaryPos == std::string::npos) {
		_response.setResponseCode(400, 
			"missing boundary parameter: " + _contentType);
		return EXIT_FAILURE;
	}

	_boundary = _boundary = "--" + _contentType.substr(boundaryPos + 9);
	if (_boundary == "--" || _boundary.length() > MAX_BOUNDARY_SIZE){
		_response.setResponseCode(400, "invalid boundary parameter: " + _contentType);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

size_t Post::countBoundaries(t_lines &lines)
{
	size_t n= 0;
	
	for (size_t i = 0; i < lines.size(); i++)
	{
		if (lines[i] == _boundary)
			n++;
	}
	return n;
}

int Post::parseContent()
{
	t_lines	lines = split< t_lines >(_request.getBody(), CRLF);
	while (lines.size() > 0 && lines.back() == "")
		lines.pop_back();
	if (parseBoundary())
		return EXIT_FAILURE;
	if (countBoundaries(lines) != 1
		|| lines.front() != _boundary || lines.back() != _boundary + "--"){
		_response.setResponseCode(400, "invalid multipart structure: only one file is allowed");
		return EXIT_FAILURE;
	}
	lines.pop_front();
	lines.pop_back();
	_content = Message(lines);
	if (_content.fail()){
		_response.setResponseCode(400, "invalid content");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}


void Post::handleNewRessource()
{
	if ( !_ressource.getExtension().empty() && getMimeType()=="application/octet-stream"){
		_response.setResponseCode(415,
			"unsupported media type: " + _ressource.getPath().litteral());
		return ;
	}
	_contentType = _request.getHeader("Content-Type");
	if (_contentType.empty()){
		_response.setResponseCode(400, "content type is missing");
		return;
	}
	else if (_contentType.find("multipart/form-data") != 0){
		_response.setResponseCode(415, _contentType + " is not supported");
		return ;
	}
	if (!parseContent() && validateContent())
		uploadFile();
}

void Post::handleExistingRessource()
{
	if (_ressource.getPath().isDir())
	{
		_response.setResponseCode(405, "POST requests are not allowed on directories"); //ok
		_response.setHeader("Allow", concatStrVec(_route.getAllowedMethods(), ", ", true));
	}
	else if (_ressource.getPath().isFile())
		_response.setResponseCode(409, _ressource.getPath().litteral() + " resource already exists: "
			+ _ressource.getPath().litteral());
	else
		_response.setResponseCode(500, "Unknown type of file");
}

bool Post::validateUploads()
{
	if (!_route.isUploadsEnabled())
	{
		_response.setResponseCode(403, "uploads are not enabled in this route");
		return false;
	}

	Path	uploadDir = Path(_route.getUploads());
	if (!uploadDir.exist()){
		_response.setResponseCode(404, "upload directory does not exist");
		return false;
	}
	else if (!uploadDir.isDir()){
		_response.setResponseCode(500, "upload directory is not a directory");
		return false;
	}
	else if (!uploadDir.writable()){
		_response.setResponseCode(403, "upload directory is not writeable");
		return false;
	}
	return true;
}

void Post::changeRessourcePath()
{
	_ressource = Ressource(Path(_route.getUploads()) + _request.getURI().getPath());
}

int Post::handleUploads()
{
	if (!validateUploads())
		return EXIT_FAILURE;
	changeRessourcePath();
	if (!_ressource.getPath().exist())
		handleNewRessource();
	else
		handleExistingRessource();
	return EXIT_SUCCESS;
}

int Post::setResponse()
{
	if (_ressource.isCgi())
		return executeCgi();
	return handleUploads();
}
