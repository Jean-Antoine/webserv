/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:07:40 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/11/14 13:55:57 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <string>
#include <set>

enum Method
{
	GET,
	POST,
	DELETE
};

typedef struct s_redir
{
	bool					enabled;
	std::string				url;
}							t_redir;

typedef struct s_cgi
{
	bool					enabled;
	std::string				path_info;
	std::set<std::string>	extensions;
}							t_cgi;

typedef struct s_upload
{
	bool					enabled;
	std::string				directory;
}							t_upload;

class Route
{
	public:
		Route();
		~Route();

	private:
		std::string 		path;
		std::set<Method>	methods;
		std::string			root;
		std::string			index;
		bool				directory_listing;
		t_redir				redirection;
		t_cgi				cgi;
		t_upload			uploads;
};


#endif
