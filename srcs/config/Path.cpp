/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Path.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeada-si <jeada-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 13:46:37 by jeada-si          #+#    #+#             */
/*   Updated: 2025/01/10 09:35:05 by jeada-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Path.hpp"

Path::Path()
{
	
}

Path::Path(const std::string path)
{
	_path = split<t_strVec>(path, "/");
	this->normalise();
	this->getStats();
}

Path::Path(const char *path)
{
	std::string	strPath = path;
	*this = Path(strPath);
}

Path::Path(const Path &src)
{
	*this = src;
}

Path& Path::operator=(const Path &src)
{
	_path = src._path;
	_stats = src._stats;
	return *this;
}

Path &	Path::operator=(const std::string &src)
{
	*this = Path(src);
	return *this;
}

Path::~Path()
{
}

void	Path::normalise()
{
	for (t_strVec::iterator it = _path.begin();
		it != _path.end(); )
	{
		if (it->empty())
			it = _path.erase(it);
		else if (*it == ".." && it != _path.begin())
			it = _path.erase(it - 1, it + 1);
		else
			it++;
	}
}

const t_strVec&	Path::get() const
{
	return _path;
}

void	Path::insert(t_strVec::const_iterator begin,
	t_strVec::const_iterator end)
{
	this->_path.insert(this->_path.end(), begin, end);
	this->normalise();
	this->getStats();
}

Path	Path::operator+(const Path & obj) const
{
	Path	out = *this;

	out.insert(obj.get().begin(), obj.get().end());
	return out;
}

std::string	Path::basename() const
{
	if (_path.empty())
		return "";
	return *(_path.rbegin());
}

std::string	Path::extension() const
{
	std::string	base = basename();
	size_t		pos = base.find_last_of('.');
	
	if (pos == std::string::npos)
		return "";
	return base.substr(pos + 1, std::string::npos);
}

std::string	Path::litteral() const
{
	std::string	out = concatStrVec(_path, "/", true);

	if (_path.empty() || _path[0] != ".")
		out.insert(out.begin(), '/');
	return out;
}

const std::string &	Path::operator[](size_t i) const
{
	return _path[i];
}

bool	Path::in(const Path & src) const
{
	size_t	i = -1;

	while (++i < size())
	{
		if (i >= src.size())
			return false;
		if ((*this)[i] != src[i])
			return false;
	}
	return true;
}

size_t	Path::compare(const Path & src)
{
	size_t	i = 0;

	if (src.size() == 0 &&
		size() == 0)
		return 1;
	while (i < size() && i < src.size())
	{
		if ((*this)[i] != src[i])
			break ;
		i++;
	}
	return i;
}

void	Path::trim(const Path & root)
{
	if (!root.in(*this))
		return ;
	for (t_strVec::const_iterator it = root.get().begin();
		it != root.get().end(); it++)
	{
		if (*it != this->_path[0])
			return ;
		_path.erase(_path.begin());
	}
}

std::ostream&	operator<<(std::ostream & out, const Path & path)
{
	out << path.litteral();
	return out;
}

bool	Path::throughParent() const
{
	long	i = 0;

	for (t_strVec::const_iterator it = _path.begin();
		it != _path.end(); it++)
	{
		if (*it == "..")
			i--;
		else
			i++;
		if (i < 0)
			return true;
	}
	return false;
}

bool	Path::readable() const
{
	return access(litteral().c_str(), R_OK) == 0;
}

size_t	Path::size() const
{
	return _path.size();
}

int	Path::getStats()
{
	return stat(litteral().c_str(), &_stats);
}

bool	Path::isDir() const
{
	return S_ISDIR(_stats.st_mode);
}

bool	Path::isFile() const
{
	return S_ISREG(_stats.st_mode);
}

off_t	Path::fileSize() const
{
	return _stats.st_size;
}

timespec	Path::fileLastModified() const
{
	return _stats.st_mtim;
}

std::string	Path::fileLastModifiedStr() const
{
	char	buffer[100];
	
	strftime(buffer, 100, "%y-%b-%d %H:%M:%S", localtime(&_stats.st_mtim.tv_sec));
	return buffer;
}

bool	Path::exist() const
{
	return access(litteral().c_str(), F_OK) == 0;
}