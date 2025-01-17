/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 12:56:57 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/15 09:38:16 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LOGS_HPP__
# define __LOGS_HPP__
# include <string>
# include <fstream>
# include "colors.hpp"
# include "utils.hpp"
# define VERBOSE true

class Client;
class Logs
{
	private:
		std::string			_color;
	public:
		std::ostream		&_out;
		void				printDate(int reset) const;
		const std::string	getColor(bool bold) const;
		void				printTab(int reset) const;
							Logs(char const* color);
							Logs(char const* color, bool error);
							~Logs();
};

const Logs &	operator<<(const Logs & logs, int i);
const Logs &	operator<<(const Logs & logs, const std::string & str);
const Logs &	operator<(const Logs & logs, int i);
const Logs &	operator<(const Logs & logs, const std::string & str);
const Logs &	operator<<(const Logs & logs, Client & clt);
int				error(const char *prefix);

#endif