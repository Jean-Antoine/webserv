/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:29:38 by jeada-si          #+#    #+#             */
/*   Updated: 2024/12/03 13:30:39 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __URI_HPP__
# define __URI_HPP__
# include "utils.hpp"

class URI
{
	private:
		std::string	_raw;
		std::string	_host;
		std::string	_port;
		std::string	_path;
		std::string	_query;
	public:
							URI();
							URI(const char* uri);
							URI(const URI &src);
							~URI();
		URI&				operator=(const URI &src);
		const std::string &	getPath() const;
};

#endif