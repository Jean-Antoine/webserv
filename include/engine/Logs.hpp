/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 12:56:57 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/06 11:23:34 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LOGS_HPP__
# define __LOGS_HPP__
# include <string>
# include <fstream>
# include "utils.hpp"
# include "colors.hpp"
# define VERBOSE true

class Client;
class Logs
{
	private:
		std::string			_color;
	public:
		void				printDate(int reset) const;
		const std::string	getColor(bool bold) const;
							Logs(char const* color);
							~Logs();
};

const Logs &	operator<<(const Logs & logs, int i);
const Logs &	operator<<(const Logs & logs, const std::string & str);
const Logs &	operator<(const Logs & logs, int i);
const Logs &	operator<(const Logs & logs, const std::string & str);

#endif