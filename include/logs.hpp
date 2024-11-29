/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaquatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:28:20 by lpaquatt          #+#    #+#             */
/*   Updated: 2024/11/28 19:43:48 by lpaquatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGS_HPP
# define LOGS_HPP

# include <iostream>
# include <string>

static const std::string RED = "\033[1;31m";
static const std::string GREEN = "\033[1;32m";
static const std::string CYAN = "\033[1;36m";
static const std::string MAGENTA = "\033[1;35m";
static const std::string YELLOW = "\033[1;33m";
static const std::string GREY = "\033[1;30m";
static const std::string WHITE = "\033[1;37m";
static const std::string ORANGE = "\033[1;38;5;208m";
static const std::string PURPLE = "\033[1;38;5;129m";
static const std::string BLUE = "\033[1;34m";
static const std::string RESET = "\033[0m";

void	log(const std::string &message);
void	testLog(const std::string &message);
int		putError(const std::string &errorMessage, int code = 1);

#endif