/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:36:15 by lpaquatt          #+#    #+#             */
/*   Updated: 2025/01/16 09:00:40 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Post.hpp"

Post::Post(Config &config,  Request & request):
	AMethod::AMethod(config, request)
{
}

//a faire
void Post::uploadFile()
{
	//changer le path avec la route
	// if (_ressource.createFile()
	// 	|| _ressource.writeInFile(_content.getBody()))
	// 	_response.setResponseCode(500, 
	// 		"error while uploading file" + _ressource.getPath().litteral());
	// else
		_response.setResponseCode(201, "file uploaded successfully");
}

bool Post::isValidContent()
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
	if (getMimeType() != _content.getHeader("Content-Type")){
		_response.setResponseCode(400, "conflicting content types in upload of " + _ressource.getPath().litteral());
		return false;
	}
	if (_content.getBody().length() > 50000) //todo @leontinepaq valeur arbitraire
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
	if (_boundary == "--" || _boundary.length() > 70){ //todo @leontinepaq valeur arbitraire
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
	if (parseBoundary())
		return EXIT_FAILURE;
	if (countBoundaries(lines) != 2
		|| lines.front() != _boundary || lines.back() != _boundary){
		_response.setResponseCode(400, "invalid multipart structure: multiple files are not allowed");
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
	if (getMimeType().empty()){
		_response.setResponseCode(415,
			"unsupported media type: " + _ressource.getPath().litteral());
		return ;
	}
	_contentType = _request.getHeader("Content-Type");
	if (_contentType.empty()){
		_response.setResponseCode(400, "content type is missing");
		return;
	}
	else if (_contentType.find("multipart/form-data") != std::string::npos
		|| _contentType.find("multipart/form-data") != 0)
		_response.setResponseCode(415, _contentType + " is not supported");
	if (!parseContent() && isValidContent())
		uploadFile();
}

std::string Post::getResponse()
{
	if (!_ressource.getPath().exist())
		handleNewRessource();
	else if (_ressource.getPath().isDir()){
		_response.setResponseCode(405, "POST requests are not allowed on directories");
		_response.setHeader("Allow", concatStrVec(_route.getAllowedMethods(), ", ", true));
	}
	else if (_ressource.isCgi())
		executeCgi();
	else if (_ressource.getPath().isFile())
		_response.setResponseCode(409, _ressource.getPath().litteral() + " resource already exists: "
			+ _ressource.getPath().litteral());
	else
		_response.setResponseCode(500, "Unknown type of file");	
	return _response.getResponse(_config);
}

// else if (_route.isRedirectionEnabled())
// 		setRedirection(); 
// todo @leontinepaq checker si utile en Post