/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 10:22:35 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/09 18:03:05 by lpaquatt         ###   ########.fr       */
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
		std::string					_requestMethod;
		std::string					_requestBody;
		int							_cgiOutputPipe[2]; // todo @leon changer en _outputPipe ?
		int							_requestBodyPipe[2]; //  _inputPipe ??
		int							_fail;
		std::string					_out;
		std::string					_body;
		t_headers					_headers;
		int							child();
		int							readCgiOutput();
		int							writeRequestBody();
	public:
									CGI(const Request 		& request,
										const std::string	& localPath,
										const std::string	& binPath);
									~CGI();
		int							execute();
		const std::string &			get() const;
		const std::string &			getBody() const;
		const t_headers &			getHeaders() const;
};

#endif