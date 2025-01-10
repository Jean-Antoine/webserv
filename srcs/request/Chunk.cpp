/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 09:43:15 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/10 10:11:46 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chunk.hpp"
#include <iostream>

Chunk::Chunk()
{
	_fail = false;
}

Chunk::Chunk(std::string raw)
{
	t_lines	lines = split < t_lines > (raw, CRLF);
	*this = Chunk(lines);
}

Chunk::Chunk(t_lines lines)
{
	_fail = parseChunks(lines);
}

Chunk::Chunk(const Chunk &src)
{
	*this = src;
}

Chunk& Chunk::operator=(const Chunk &src)
{
	_fail = src._fail;
	_body = src._body;
	return *this;
}

Chunk::~Chunk()
{
}

bool Chunk::isChunkLastLine(t_lines &lines) const
{
	return lines.size() == 1 && lines.front().empty();
}

bool Chunk::isEndOfChunks(t_lines &lines)
{
	_last = (lines[0] == "0" && lines[1].empty());
	return _last;
}

bool Chunk::isValidChunk(t_lines &lines) const
{
	size_t chunkSize;
	
	if (convertHexa(lines[0], chunkSize)
		|| lines[1].size() != chunkSize)
		return false;
	return true;
}

int Chunk::parseChunks(t_lines &lines)
{
	_last = false;
	while (lines.size() > 1)
	{
		if (isEndOfChunks(lines))
			return EXIT_SUCCESS;
		if (!isValidChunk(lines))
			return EXIT_FAILURE;
		_body.append(lines[1]);
		lines.pop_front();
		lines.pop_front();
	}
	return !isChunkLastLine(lines);
}

bool				Chunk::fail() const
{
	return _fail;
}

bool				Chunk::last() const
{
	return _last;
}

const std::string & Chunk::body() const
{
	return _body;
}
