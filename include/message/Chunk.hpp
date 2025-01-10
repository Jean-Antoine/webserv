/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 09:42:08 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/10 16:03:43 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CHUNK_HPP__
# define __CHUNK_HPP__
# define CRLF "\r\n"
# include <string>
# include <deque>
# include "utils.hpp"
# include "Logs.hpp"

typedef	std::deque < std::string >			t_lines;

class Chunk
{
	private:
		std::string			_body;
		bool				_fail;
		bool				_complete;
		int					parseChunks(t_lines &lines);
		bool				isEndOfChunks(t_lines &lines);
		bool				isLastLine(t_lines &lines) const;
		bool				isValidChunk(t_lines &lines) const;
	public:
							Chunk();
							Chunk(std::string raw);
							Chunk(t_lines lines);
							Chunk(const Chunk &src);
							~Chunk();
		Chunk& 				operator=(const Chunk &src);
		bool				fail() const;
		bool				complete() const;
		const std::string &	body() const;
};

#endif