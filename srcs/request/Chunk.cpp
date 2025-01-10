/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 09:43:15 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/10 15:14:38 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chunk.hpp"
#include <iostream>

Chunk::Chunk():
	_fail(false),
	_complete(false)
{
}

Chunk::Chunk(std::string raw):
	_fail(false),
	_complete(false)
{
	t_lines	lines = split < t_lines > (raw, CRLF);
	_fail = parseChunks(lines);
	if (_fail)
		Logs(RED) < "Chunk parsing failed\n";
}

Chunk::Chunk(t_lines lines):
	_fail(false),
	_complete(false)
{
	_fail = parseChunks(lines);
	if (_fail)
		Logs(RED) < "Chunk parsing failed\n";
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

bool Chunk::isLastLine(t_lines &lines) const
{
	return lines.size() == 1 && lines.front().empty();
}

bool Chunk::isEndOfChunks(t_lines &lines)
{
	return (lines[0] == "0" && lines[1].empty());
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
	_complete = false;
	if (isLastLine(lines))
		return EXIT_SUCCESS;

	int	status = EXIT_SUCCESS;
	while (lines.size() > 1 && !_complete)
	{
		if (isEndOfChunks(lines))
			_complete = true;
		if (!isValidChunk(lines))
			status =  EXIT_FAILURE;
		_body.append(lines[1]);
		lines.pop_front();
		lines.pop_front();
	}
	if (status || !isLastLine(lines))
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

bool				Chunk::fail() const
{
	return _fail;
}

bool				Chunk::complete() const
{
	return _complete;
}

const std::string & Chunk::body() const
{
	return _body;
}
