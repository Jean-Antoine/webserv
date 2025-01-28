/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 10:22:35 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/28 07:08:40 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CGI_HPP__
# define __CGI_HPP__
# include <vector>
# include <unistd.h>
# include <string>
# include <wait.h>
# include <cstdlib>
# include <fstream>
# include <unistd.h>
# include "Request.hpp"

# define READ 0
# define WRITE 1
# define TIME_OUT_SEC 5 //nginx default timeout = 60 s

extern int	g_run;
extern int	g_exitStatus;

class CGI
{
	private:
		std::vector < std::string >	_env;
		std::vector < std::string >	_args;
		std::string					_requestMethod;
		std::string					_requestBody;
		int							_out[2];
		int							_in[2];
		std::string					_read;
		pid_t						_pid;
		time_t						_start;
		bool						_fail;
		bool						_readComplete;
		int							child();
	public:
									CGI();
									CGI(const Request 		& request,
										const std::string	& scriptPath,
										const std::string	& binPath);
		const CGI &					operator=(const CGI & src);
									~CGI();
		int							run();
		int							readFrom();
		int							writeTo();
		int							execComplete()const;
		int							execFailed() const;
		int							readComplete() const;
		int							execTimeout() const;
		int							complete() const;
		int							getPipeIn() const;
		int							getPipeOut() const;
		Message						getResult() const;
		int							killChild();
		int							ioFailed() const;
		void						setReadComplete(int fd);
};

#endif