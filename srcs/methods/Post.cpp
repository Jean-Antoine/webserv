/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:36:15 by lpaquatt          #+#    #+#             */
/*   Updated: 2025/01/07 14:33:43 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Post.hpp"

Post::Post(Config *config,  Request & request):
	AMethod::AMethod(config, request)
{
}

std::string Post::getResponse()
{
	if (!isValid() || _ressource.isCgi())
		return _response.getResponse();
	// std::string path = _ressource.getPath();
	
	return _response.getResponse();
}
