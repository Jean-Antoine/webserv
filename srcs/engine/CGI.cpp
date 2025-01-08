/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 10:23:59 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/07 12:08:27 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CGI.hpp"

CGI::CGI(const std::string & query,
	const std::string & uriPath,
	const std::string & method,
	const std::string & localPath,
	const std::string & httpVersion,
	const std::string & binPath)
{
	_fail = false;
	_env.push_back(std::string("QUERY_STRING=" + query));
	_env.push_back(std::string("SCRIPT_NAME=" + uriPath));
	_env.push_back(std::string("REQUEST_METHOD=" + method));
	_env.push_back(std::string("PATH_INFO=" + localPath));
	_env.push_back(std::string("SCRIPT_FILENAME=" + localPath));
	_env.push_back(std::string("SERVER_PROTOCOL=" + httpVersion));
	_env.push_back(std::string("GATEWAY_INTERFACE=CGI/1.1"));
	_env.push_back(std::string("REDIRECT_STATUS=200"));
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
	close(_fd[0]);
	if (dup2(_fd[1], STDOUT_FILENO) == -1)
		return EXIT_FAILURE;
	close(_fd[1]);
	execve(_argv[0], _argv.data(), _envp.data());
	close(STDOUT_FILENO);
	return EXIT_FAILURE;
}

int	CGI::readPipe()
{
	char		buffer[1000];
	int			nread;
	
	while ((nread = read(_fd[0], buffer, 1000)))
	{
		if (nread == -1)
			return EXIT_FAILURE;
		buffer[nread] = 0;
		_out.append(buffer);
	}
	close(_fd[0]);
	return EXIT_SUCCESS;
}

int	CGI::execute()
{
	int		status;
	pid_t	pid;
	
	if (pipe(_fd) == -1)
		return EXIT_FAILURE;
	pid = fork();
	if (pid == -1)
		return EXIT_FAILURE;
	if (pid == 0)
		return (child());
	close(_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status))
		return EXIT_FAILURE;
	if(readPipe())
		return EXIT_FAILURE;
	Request	request(_out.c_str(), true);
	_body = request.getBody();
	_headers = request.getHeaders();
	return EXIT_SUCCESS;
}

const std::string &	CGI::get() const
{
	return _out;
}

const std::string &	CGI::getBody() const
{
	return _body;
}

const t_headers &	CGI::getHeaders() const
{
	return _headers;
}