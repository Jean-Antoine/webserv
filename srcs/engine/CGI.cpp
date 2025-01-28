/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 10:23:59 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/28 11:17:23 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CGI.hpp"
# include "Logs.hpp"
# include "Client.hpp"

static void closeFd(int *fd)
{
	if (*fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
}

CGI::CGI()
{
	_out[READ] = -1;
	_out[WRITE] = -1;
	_in[READ] = -1;
	_in[WRITE] = -1;
	_pid = -1;
	_fail = false;
	_readComplete = false;
}

CGI::CGI(const Request & request,
	const std::string & scriptPath,
	const std::string & binPath)	
{
	_out[READ] = -1;
	_out[WRITE] = -1;
	_in[READ] = -1;
	_in[WRITE] = -1;
	_pid = -1;
	_fail = false;
	_readComplete = false;
	_requestMethod = request.getMethod();

	_env.push_back("QUERY_STRING=" + request.getURI().getQuery());
	_env.push_back("SCRIPT_NAME=" + request.getURI().getPath());
	_env.push_back("REQUEST_METHOD=" + _requestMethod);
	_env.push_back("PATH_INFO=" + scriptPath);
	_env.push_back("SCRIPT_FILENAME=" + scriptPath);
	_env.push_back("SERVER_PROTOCOL=" + request.getHttpVersion());
	_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	_env.push_back("REDIRECT_STATUS=200");
	if (!request.getHeader("Cookie").empty())
		_env.push_back("HTTP_COOKIE=" + request.getHeader("Cookie"));
	if (_requestMethod == "POST")
	{
		_env.push_back("CONTENT_LENGTH=" + request.getHeader("Content-Length"));
		_env.push_back("CONTENT_TYPE=" + request.getHeader("Content-Type"));
		_requestBody = request.getBody();
	}
	_args.push_back(binPath);
	_args.push_back(scriptPath);
}

const CGI &	CGI::operator=(const CGI & src)
{
	_env = src._env;
	_args = src._args;
	_requestBody = src._requestBody;
	_requestMethod = src._requestMethod;
	_out[0] = src._out[0];
	_out[1] = src._out[1];
	_in[0] = src._in[0];
	_in[1] = src._in[1];
	_read = src._read;
	_pid = src._pid;
	_fail = src._fail;
	return *this;
}

static void closePipe(int *pipe)
{
	closeFd(pipe + READ);
	closeFd(pipe + WRITE);
}

CGI::~CGI()
{
	closePipe(_out);
	closePipe(_in);
}

static void	setPointerTab(t_strVec & src, std::vector  < char * > & dest)
{	
	for (std::vector < std::string >::iterator it = src.begin();
		it != src.end(); it++)
			dest.push_back(const_cast < char * >(it->c_str()));
	dest.push_back(NULL);
}

int CGI::writeTo()
{
	ssize_t		nwrite;
	
	if (_in[WRITE] == -1)
		return EXIT_FAILURE;
	nwrite = write(_in[WRITE], _requestBody.c_str(), _requestBody.size());
	closeFd(&_in[WRITE]);
	if (nwrite == -1 || nwrite != (ssize_t) _requestBody.size())
	{
		closeFd(&_in[WRITE]);
		_fail = true;
		return error("write");
	}
	return EXIT_SUCCESS;
}

int	CGI::readFrom()
{
	char		buffer[4096];
	int			nread;
	
	if (_out[READ] == -1)
		return EXIT_FAILURE;
	nread = read(_out[READ], buffer, 4096);
	if (nread <= 0)
	{
		closeFd(&_out[READ]);
		_fail = true;
		return error("read");
	}
	_read.append(buffer, nread);
	return EXIT_SUCCESS;
}

int CGI::killChild()
{
	return kill(_pid, SIGKILL);
}

int	CGI::execComplete() const
{
	int	status = 0;
	
	if (waitpid(_pid, &status, WNOHANG) == 0)	
		return false;
	if (WIFEXITED(status))
		return WEXITSTATUS(status) == EXIT_SUCCESS;
	return false;
}

int	CGI::execFailed() const
{
	int status = 0;	
	
	if (waitpid(_pid, &status, WNOHANG) == 0)
		return false;
	if (WIFEXITED(status))
		return WEXITSTATUS(status) != EXIT_SUCCESS;
	return false;
}

int	CGI::ioFailed() const
{
	return _fail;
}

int	CGI::run()
{
	_start = std::time(NULL);
	if (pipe(_out) == -1
		|| (_requestMethod == "POST" && pipe(_in) == -1))
		return error("pipe");
	if (setNonBlocking(_out[READ])
		|| (_requestMethod == "POST" && setNonBlocking(_in[WRITE])))
		return EXIT_FAILURE;
	_pid = fork();
	if (_pid == -1)
		return error("fork");
	if (_pid == 0)
		return child();
	closeFd(&_out[WRITE]);
	closeFd(&_in[READ]);
	return EXIT_SUCCESS;
}

int	CGI::child()
{
	std::vector < char * >	argv;
	std::vector < char * >	envp;
	
	g_run = false;
	g_exitStatus = EXIT_FAILURE;
	setPointerTab(_env, envp);
	setPointerTab(_args, argv);
	if (dup2(_out[WRITE], STDOUT_FILENO) == -1 
		|| (_requestMethod == "POST" && dup2(_in[READ], STDIN_FILENO) == -1))
		return error("dup2");
	closePipe(_in);
	closePipe(_out);
	execve(argv[0], argv.data(), envp.data());
	return EXIT_FAILURE;
}

Message	CGI::getResult() const
{
	return Message(_read);
}

int	CGI::readComplete() const
{
	if (_readComplete)
		return true;
	Message message(_read);
	return message.complete();
}

int	CGI::complete() const
{
	return execComplete() && _readComplete;
}

int CGI::execTimeout() const
{	
	return waitpid(_pid, NULL, WNOHANG) == 0
		&& std::difftime(std::time(NULL), _start) > TIME_OUT_SEC;
}

int	CGI::getPipeIn() const
{
	return _in[WRITE];
}

int	CGI::getPipeOut() const
{
	return _out[READ];
}

void	CGI::setReadComplete(int fd)
{
	if (_out[READ] == fd)
		_readComplete = true;
}