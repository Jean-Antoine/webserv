/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 10:23:59 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/19 00:02:58 by lpaquatt         ###   ########.fr       */
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

int CGI::monitorTimeout()
{
	fd_set			readfds;
	struct timeval	timeout;

	FD_ZERO(&readfds);
	FD_SET(_cgiOutputPipe[READ], &readfds);

	timeout.tv_sec = TIME_OUT_SEC;
	timeout.tv_usec = 0;

	int ret = select(_cgiOutputPipe[READ] + 1, &readfds, NULL, NULL, &timeout);
	if (ret == -1)
		return error("select");
	else if (ret == 0) { // Timeout
		kill(_pidChild, SIGKILL);
		// _response.setResponseCode(408, "CGI timeout"); // todo @leontinepaq ajouter les codes d'erreur + ajouter un test
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int CGI::monitorChild()
{
	int		status;

	closeFd(_cgiOutputPipe + WRITE);
	closeFd(_cgiInputPipe + READ);
	if (_requestMethod == "POST")
		if (writeToCgiInput())
			return EXIT_FAILURE;
	closeFd(_cgiInputPipe + WRITE);

	if (monitorTimeout())
	{
		waitpid(_pidChild, &status, 0);
		return EXIT_FAILURE;
	}
	if (waitpid(_pidChild, &status, 0) == -1)
        return error("waitpid");
	if (WIFEXITED(status) && WEXITSTATUS(status))
		return EXIT_FAILURE; // todo @leontinepaq ajouter les codes d'erreur / message / est-ce que c'est un fail ?
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
