/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 10:22:35 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/19 01:20:16 by lpaquatt         ###   ########.fr       */
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
# define TIME_OUT_SEC 5

extern int	g_run;
extern int	g_exitStatus;

class CGI
{
	private:
		std::vector < std::string >	_env;
		std::vector < std::string >	_args;
		std::vector < char * >		_envp;
		std::vector < char * >		_argv;
		std::string					_requestMethod;
		std::string					_requestBody;
		int							_cgiOutputPipe[2];
		int							_cgiInputPipe[2];
		int							_fail;
		std::string					_out;
		Message						_message;
		pid_t						_pidChild;
		
		int							child();
		int							monitorChild();
		int							readCgiOutput();
		int							writeToCgiInput();
		int 						monitorTimeout();

	public:
									CGI(const Request 		& request,
										const std::string	& localPath,
										const std::string	& binPath);
									~CGI();
		int							execute();
		const Message &				get() const;
};

#endif