/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:48:27 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/16 09:05:04 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Delete.hpp"

Delete::Delete(Config &config, Request &request):
	AMethod::AMethod(config, request)
{
}

Delete::~Delete()
{
}

std::string	Delete::getResponse()
{
	return "";
}