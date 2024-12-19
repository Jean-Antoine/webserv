/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 19:21:14 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/18 17:37:05 by lpaquatt         ###   ########.fr       */
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
	_response.setResponseCode(405, _request.getMethod());
	_response.setHeader("Allow", concatStrVec(_route.getAllowedMethods(), ", ", true));
	return false;	
}

bool	AMethod::validateMethod()
{
	if (_request.getMethod() == "INVALID")
		return _response.setResponseCode(501, "Implemented methods are GET, POST and DELETE");
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
		return _response.setResponseCode(404, "Route not found");
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
		return _response.setResponseCode(400, "Invalid Http Version format");
	if (httpVersion != "HTTP/1.1")
	{
		_response.setHeader("Connection", "close");
		return _response.setResponseCode(505, httpVersion);
	}
	return true;
}

std::string AMethod::getMimeType(const std::string & path)
{
	return _config->getMimeType(getExtension(path));
}

void AMethod::createCgiEnvp(char *envp[])
{
	std::string	query = _request.getURI().getQuery();	
	
	if (_request.getMethod() == "POST")
		query = _request.getBody();

	std::string	query_string =		"QUERY_STRING=" + query;
	std::string script_name =		"SCRIPT_NAME=" + _request.getURI().getPath();
	std::string request_method =	"REQUEST_METHOD=" + _request.getMethod();
	std::string path_info =			"PATH_INFO=" + _route.getLocalPath();
	std::string server_protocol =	"SERVER_PROTOCOL=" + _request.getHttpVersion();
	std::string gateway_interface =	"GATEWAY_INTERFACE=CGI/1.1";
	std::string script_filename =	"SCRIPT_FILENAME=" + _route.getLocalPath();
	std::string redirect_status =	"REDIRECT_STATUS=200";
	// todo: add content length and type (mandatory for post ..?)

	envp[0] = &query_string[0];
	envp[1] = &script_name[0];
	envp[2] = &request_method[0];
	envp[3] = &path_info[0];
	envp[4] = &server_protocol[0];
	envp[5] = &gateway_interface[0];
	envp[6] = &script_filename[0];
	envp[7] = &redirect_status[0];
	envp[8] = NULL;
}

void AMethod::createCgiArgv(char *argv[])
{
	std::string binPath = _route.getCgiBinPath();
	std::string filePath = _route.getLocalPath();

	argv[0] = &binPath[0];
	argv[1] = &filePath[0];
	argv[2] = NULL; 
}

int	AMethod::execCgi(std::string &dest) //pas trop sure de ce qu'est la dest si ca ecrit toute la reponse
{
	char	*envp[9];
	createCgiEnvp(envp);
	int	fd[2];
	if (pipe(fd) == -1)
		return !_response.setResponseCode(500, "PIPE");
	
	pid_t pid = fork();
	if (pid == -1)
        return !_response.setResponseCode(500, "FORK");

	if (pid == 0)
	{
		dup2(fd[WRITE], STDOUT_FILENO);
		close(fd[READ]);
		char* argv[3];
		createCgiArgv(argv);
		execve(_route.getCgiBinPath().c_str(), argv, envp);
		perror("execve"); // todo: a gerer
        exit(EXIT_FAILURE); //
	}
	else
	{
		close(fd[WRITE]);
		char buffer[1024]; // todo: define
		ssize_t bytesRead;

		while ((bytesRead = read(fd[READ], buffer, sizeof(buffer))) > 0)
			dest.append(buffer, bytesRead);
		close(fd[0]);
		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			return !_response.setResponseCode(500, "CGI");
        return EXIT_SUCCESS;
	}
}
