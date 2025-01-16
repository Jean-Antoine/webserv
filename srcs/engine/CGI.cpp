/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 10:23:59 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/16 18:00:35 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CGI.hpp"

CGI::CGI(const Request & request,
	const std::string & localPath,
	const std::string & binPath,
	const std::string & uploadsDir)
{
	(void) uploadsDir;
	
	_fail = false;
	_requestMethod = request.getMethod();
	_env.push_back("QUERY_STRING=" + request.getURI().getQuery());
	_env.push_back("SCRIPT_NAME=" + request.getURI().getPath());
	_env.push_back("REQUEST_METHOD=" + _requestMethod);
	_env.push_back("PATH_INFO=" + localPath);
	_env.push_back("SCRIPT_FILENAME=" + localPath);
	_env.push_back("SERVER_PROTOCOL=" + request.getHttpVersion());
	_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	_env.push_back("REDIRECT_STATUS=200");
	if (_requestMethod == "POST")
	{
		_env.push_back("CONTENT_LENGTH=" + request.getHeader("Content-Length"));
		_env.push_back("CONTENT_TYPE=" + request.getHeader("Content-Type"));
		_requestBody = request.getBody();
	}
	_args.push_back(binPath);
	_args.push_back(localPath);

	for (std::vector < std::string >::iterator it = _env.begin();
		it != _env.end(); it++)
			_envp.push_back(const_cast < char * >(it->c_str()));
	_envp.push_back(NULL);
	for (std::vector < std::string >::iterator it = _args.begin();
		it != _args.end(); it++)
			_argv.push_back(const_cast < char * >(it->c_str()));
	_argv.push_back(NULL);
}

CGI::~CGI()
{
}

int	CGI::child()
{
	g_run = false;
	g_exitStatus = EXIT_FAILURE;
	if (dup2(_cgiOutputPipe[1], STDOUT_FILENO) == -1 
		|| dup2(_requestBodyPipe[0], STDIN_FILENO) == -1)
		return EXIT_FAILURE;
	close(_cgiOutputPipe[0]);
	close(_cgiOutputPipe[1]);
	close(_requestBodyPipe[0]);
	close(_requestBodyPipe[1]);
	execve(_argv[0], _argv.data(), _envp.data());
	close(STDOUT_FILENO);
	return EXIT_FAILURE;
}

int CGI::writeRequestBody()
{
	ssize_t		nwrite;
	
	nwrite = write(_requestBodyPipe[1], _requestBody.c_str(), _requestBody.size());
	if (nwrite == -1 || static_cast<size_t>(nwrite) != _requestBody.size())
		return EXIT_FAILURE;
	close(_requestBodyPipe[1]);
	return EXIT_SUCCESS;
}

int	CGI::readCgiOutput()
{
	char		buffer[1000];
	int			nread;
	
	while ((nread = read(_cgiOutputPipe[0], buffer, 1000)))
	{
		if (nread == -1)
			return EXIT_FAILURE;
		buffer[nread] = 0;
		_out.append(buffer, nread);
	}
	close(_cgiOutputPipe[0]);
	return EXIT_SUCCESS;
}

int	CGI::execute()
{
	int		status;
	pid_t	pid;
	
	if (pipe(_cgiOutputPipe) == -1 || pipe(_requestBodyPipe) == -1)
		return EXIT_FAILURE;
	pid = fork();
	if (pid == -1)
		return EXIT_FAILURE;
	if (pid == 0)
		return (child());
	close(_cgiOutputPipe[1]);
	close(_requestBodyPipe[0]);
	if (_requestMethod == "POST")
		if (writeRequestBody())
			return EXIT_FAILURE;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status))
		return EXIT_FAILURE;
	if(readCgiOutput())
		return EXIT_FAILURE;
	_message = Message(_out.c_str());
	return EXIT_SUCCESS;
}

const Message &	CGI::get() const
{
	return _message;
}
