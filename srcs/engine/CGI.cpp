/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 10:23:59 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/23 01:02:48 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CGI.hpp"
# include "Logs.hpp"

static void closeFd(int *fd)
{
	if (*fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
}

static void closePipe(int *pipe)
{
	closeFd(pipe + READ);
	closeFd(pipe + WRITE);
}

CGI::CGI(const Request & request,
	const std::string & localPath,
	const std::string & binPath)
{
	_cgiOutputPipe[READ] = -1;
	_cgiOutputPipe[WRITE] = -1;
	_cgiInputPipe[READ] = -1;
	_cgiInputPipe[WRITE] = -1;
	_pidChild = -1;
	_fail = false;
	_responseCode = 500;
	_requestMethod = request.getMethod();

	_env.push_back("QUERY_STRING=" + request.getURI().getQuery());
	_env.push_back("SCRIPT_NAME=" + request.getURI().getPath());
	_env.push_back("REQUEST_METHOD=" + _requestMethod);
	_env.push_back("PATH_INFO=" + localPath);
	_env.push_back("SCRIPT_FILENAME=" + localPath);
	_env.push_back("SERVER_PROTOCOL=" + request.getHttpVersion());
	_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	_env.push_back("REDIRECT_STATUS=200");
	if (!request.getHeader("Cookie").empty())
		_env.push_back("HTTP_COOKIE=" + request.getHeader("Cookie"));
	Logs(ORANGE) << "Cookie: " << request.getHeader("Cookie") << "\n";
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
	closePipe(_cgiOutputPipe);
	closePipe(_cgiInputPipe);
}

int	CGI::child()
{
	g_run = false;
	g_exitStatus = EXIT_FAILURE;
	if (dup2(_cgiOutputPipe[WRITE], STDOUT_FILENO) == -1 
		|| dup2(_cgiInputPipe[READ], STDIN_FILENO) == -1)
		return error("dup2");
	execve(_argv[READ], _argv.data(), _envp.data());
	close(STDOUT_FILENO);
	return EXIT_FAILURE;
}

int CGI::writeToCgiInput()
{
	ssize_t		nwrite;
	
	nwrite = write(_cgiInputPipe[WRITE], _requestBody.c_str(), _requestBody.size());
	if (nwrite == -1 || static_cast<size_t>(nwrite) != _requestBody.size())
		return error("write");
	return EXIT_SUCCESS;
}

int	CGI::readCgiOutput()
{
	char		buffer[1000];
	int			nread;
	
	while ((nread = read(_cgiOutputPipe[READ], buffer, 1000)))
	{
		if (nread == -1)
			return error("read");
		buffer[nread] = 0;
		_out.append(buffer, nread);
	}
	return EXIT_SUCCESS;
}

int CGI::waitForChild()
{
	int			status;
	std::time_t beg = std::time(0);
	pid_t		waitPid = 0;

	while (waitPid == 0)
	{
		std::time_t now = std::time(0);
		if (now - beg > TIME_OUT_SEC){
			kill(_pidChild, SIGKILL);
			Logs(RED) << "CGI timeout - sending SIGKILL\n"; //@Jean-Antoine on gere comme ca ?
			_responseCode = 504;
			return EXIT_FAILURE;
		}
		usleep(1000);
		waitPid = waitpid(_pidChild, &status, WNOHANG);
	}
	if (waitPid == -1)
        return error("waitpid");
	if (WIFEXITED(status) && WEXITSTATUS(status))
		return EXIT_FAILURE; // @Jean-Antoine: soit on laisse ca et on renvoie un 500 si le script fail soit on l'enleve et ca fait 200 ce qui est le comportement de nginx avec GET /webserv_test/edge_cases/error.php
	return EXIT_SUCCESS;
}

int CGI::monitorChild()
{
	closeFd(_cgiOutputPipe + WRITE);
	closeFd(_cgiInputPipe + READ);
	if (_requestMethod == "POST")
		if (writeToCgiInput())
			return EXIT_FAILURE;
	closeFd(_cgiInputPipe + WRITE);
	if (waitForChild())
		return EXIT_FAILURE;
	if(readCgiOutput())
		return EXIT_FAILURE;
	_message = Message(_out.c_str());
	return EXIT_SUCCESS;
}

int	CGI::execute()
{
	if (pipe(_cgiOutputPipe) == -1 || pipe(_cgiInputPipe) == -1)
		return error("pipe");
	_pidChild = fork();
	if (_pidChild == -1)
		return error("fork");
	if (_pidChild == 0)
		return child();
	else
		return monitorChild();
}

const Message &	CGI::get() const
{
	return _message;
}
