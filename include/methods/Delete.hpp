/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:47:34 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/16 09:39:40 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __DELETE_HPP__
# define __DELETE_HPP__
# include "AMethod.hpp"

class Config;
class Request;
class Response;
class Delete: public AMethod
{
	private:
		
	public:
			Delete(Config &config, Request & request);
			~Delete();
		int	setResponse();
};

#endif