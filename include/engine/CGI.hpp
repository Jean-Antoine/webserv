/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 10:22:35 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/18 10:36:43 by jeada-si         ###   ########.fr       */
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

extern int	g_run;
extern int	g_exitStatus;

class CGI
{
	private:
		std::vector < std::string >	_env;
		std::vector < std::string >	_args;
		std::vector < char * >		_envp;
		std::vector < char * >		_argv;
		int							_fd[2];
		int							_fail;
		std::string					_out;
		std::string					_body;
		t_headers					_headers;
		int							child();
		int							readPipe();
	public:
									CGI(const std::string & query,
										const std::string & uriPath,
										const std::string & method,
										const std::string & localPath,
										const std::string & httpVersion,
										const std::string & binPath);
									~CGI();
		int							execute();
		const std::string &			get() const;
		const std::string &			getBody() const;
		const t_headers &			getHeaders() const;
};

#endif