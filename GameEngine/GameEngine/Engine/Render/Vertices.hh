#pragma once

# include <vector>
# include <memory>

class Data;

class Vertices
{
public:
	typedef std::vector<std::shared_ptr<Data>>::iterator iterator;
	typedef std::vector<std::shared_ptr<Data>>::const_iterator const_iterator;

public:
	Vertices(std::vector<Data> const &data);
	Vertices(std::vector<Data> &&data);
	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;
	size_t offset() const;
	size_t size() const;

private:
	std::vector<std::shared_ptr<Data>> _data;
	size_t _offset;
	size_t _size;
};