#pragma once

#include <iostream>
#include <queue>
#include <cstdint>
#include <Utils/Containers/Vector.hpp>
#include <assert.h>

struct InternalData
{
	std::size_t id;
	std::queue<std::size_t> trash;
	InternalData() : id(0){}
};

template <typename TYPE>
class Key
{
public:
	static Key<TYPE> createKey() 
	{
		return (Key<TYPE>(index));
	}

public:
	size_t getId() const;
	operator bool() const;
	void destroy();

private:
	int _id;
	explicit Key(size_t id);
};

#include <Render/Key.hpp>

template <typename TYPE>
std::size_t Key<TYPE>::getId() const
{
	return (_id);
}

template <typename TYPE>
Key<TYPE>::operator bool()
{
	if (_id == -1) {
		return (false);
	}
	return (true);
}

template <typename TYPE>
void Key<TYPE>::destroy()
{
	_id = -1;
}

template <typename TYPE>
Key<TYPE>::Key(size_t id)
{
	_id = id;
}
