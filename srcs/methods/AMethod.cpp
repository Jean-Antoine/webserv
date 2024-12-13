/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 19:21:14 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/13 17:01:04 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMethod.hpp"

AMethod::AMethod(Config *config, Request & request):
	_config(config),
	_request(request)
{
	_route = _config->getRoute(_request.getURI());
	if (_request.getHeaders()["Connection"] == "close")
		_response.setHeader("Connection", "close");
	// if (isCgi())		
}

bool AMethod::isValid()
{
	if (!validateURI()
		|| !validateRoute()
		|| !validateHttpVersion()
		|| !validateMethod())
		return false;
	return true;
}

bool	AMethod::checkAllowedMethods()
{	
	if (_route.isMethodAllowed(_request.getMethod()))
		return true;
	_response.setResponseCode(405, "Method Not Allowed");
	_response.setHeader("Allow", concatStrVec(_route.getAllowedMethods(), ", ", true));
	return false;	
}

bool	AMethod::validateMethod()
{
	if (_request.getMethod() == "INVALID")
		return _response.setResponseCode(501, "Method not implemented");
	return checkAllowedMethods();
}

bool	AMethod::validateURI()
{
	if (_request.getURI().bad())
		return _response.setResponseCode(400, "Bad URI");
	return true;
}

bool	AMethod::validateRoute()
{
	if (_route.bad())
		return _response.setResponseCode(404, "Not found");
	return true;
}

bool	AMethod::validateHttpVersion()
{
	std::string &	httpVersion = _request.getHttpVersion();
	
	if( httpVersion.length() != 8
		|| httpVersion.compare(0, 5, "HTTP/") != 0
		|| !std::isdigit(httpVersion[5])
		|| httpVersion[6] != '.'
		|| !std::isdigit(httpVersion[7]))
		return _response.setResponseCode(400, "Bad request (Invalid Http Version format)");
	if (httpVersion != "HTTP/1.1")
	{
		_response.setHeader("Connection", "close");
		return _response.setResponseCode(505, "HTTP Version Not Supported");
	}
	return true;
}

std::string AMethod::getMimeType(const std::string & path)
{
	return _config->getMimeType(getExtension(path));
}

std::string	AMethod::execCgi()
{
	std::string	query = _request.getURI().getQuery();	
	
	if (_request.getMethod() == "POST")
		query = _request.getBody();
	
	std::string	query_string = "QUERY_STRING=" + query;
	std::string script_name = "SCRIPT_NAME=" + _request.getURI().getPath();
	std::string request_method = "REQUEST_METHOD=" + _request.getMethod();
	std::string path_info = "PATH_INFO=" + _route.getLocalPath();
	std::string script_filename = "SCRIPT_FILENAME=" + _route.getLocalPath();
	std::string server_protocol = "SERVER_PROTOCOL=" + _request.getHttpVersion();
	std::string gateway_interface = "GATEWAY_INTERFACE=CGI/1.1";
	std::string redirect_status = "REDIRECT_STATUS=200";
	
	char	*envp[9];
	
	envp[0] = &query_string[0];
	envp[1] = &script_name[0];
	envp[2] = &request_method[0];
	envp[3] = &path_info[0];
	envp[4] = &server_protocol[0];
	envp[5] = &gateway_interface[0];
	envp[6] = &script_filename[0];
	envp[7] = &redirect_status[0];
	envp[8] = NULL;
	
	int	fd[2];
	if (pipe(fd) == -1)
	{
		_response.setResponseCode(500, "Internal server error (PIPE)");
		return "";
	};
	if (fork() == 0)
	{
		char* argv[3];
		
		argv[0] = &(_route.getCgiBinPath()[0]);
		argv[0] = &_route.getLocalPath().c_str();
		argv[0] = NULL;
		
		execve(_route.getCgiBinPath().c_str(), argv, envp);
	}
}

std::string	AMethod::execCgi()
{
	std::string	query = _request.getURI().getQuery();	
	
	if (_request.getMethod() == "POST")
		query = _request.getBody();
	
	std::string	query_string = "QUERY_STRING=" + query;
	std::string script_name = "SCRIPT_NAME=" + _request.getURI().getPath();
	std::string request_method = "REQUEST_METHOD=" + _request.getMethod();
	std::string path_info = "PATH_INFO=" + _route.getLocalPath();
	std::string script_filename = "SCRIPT_FILENAME=" + _route.getLocalPath();
	std::string server_protocol = "SERVER_PROTOCOL=" + _request.getHttpVersion();
	std::string gateway_interface = "GATEWAY_INTERFACE=CGI/1.1";
	std::string redirect_status = "REDIRECT_STATUS=200";
	
	char	*envp[9];
	
	envp[0] = &query_string[0];
	envp[1] = &script_name[0];
	envp[2] = &request_method[0];
	envp[3] = &path_info[0];
	envp[4] = &server_protocol[0];
	envp[5] = &gateway_interface[0];
	envp[6] = &script_filename[0];
	envp[7] = &redirect_status[0];
	envp[8] = NULL;
	
	int	fd[2];
	if (pipe(fd) == -1)
	{
		_response.setResponseCode(500, "Internal server error (PIPE)");
		return "";
	};
	if (fork() == 0)
	{
		char* argv[3];
		
		argv[0] = &(_route.getCgiBinPath()[0]);
		argv[0] = &_route.getLocalPath().c_str();
		argv[0] = NULL;
		
		execve(_route.getCgiBinPath().c_str(), argv, envp);
	}
}
