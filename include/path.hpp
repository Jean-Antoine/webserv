/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 17:10:05 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/12/13 17:03:15 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATH_HPP
# define PATH_HPP

# include "utils.hpp"
# include <fstream>
# include <sys/stat.h>
# include <string>
# include <dirent.h>
# include <unistd.h>


typedef enum {FILE_PATH, DIR_PATH, OTHER_PATH} t_pathType;

t_pathType	getPathType(std::string path);
int			getDirectoryListing (std::string dirPath, t_strVec & items);
int			readFile(std::string path, std::string & dest);
std::string	concatPath(const std::string & prefix, const std::string & path);
std::string	getExtension(const std::string & path);

#endif
