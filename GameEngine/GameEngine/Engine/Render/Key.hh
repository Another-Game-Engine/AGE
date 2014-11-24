#pragma once

#include <iostream>
#include <queue>
#include <cstdint>
#include <vector>

template <typename TYPE>
class Key
{
public:
	Key();

public:
	static Key<TYPE> createKey(size_t index);

public:
	size_t getId() const;
	operator bool() const;
	void destroy();

private:
	int _id;
	explicit Key(size_t id);
};

template <typename type_t>
std::size_t Key<type_t>::getId() const
{
	return (_id);
}

template <typename type_t>
Key<type_t>::operator bool() const
{
	if (_id == -1) {
		return (false);
	}
	return (true);
}

template <typename type_t>
void Key<type_t>::destroy()
{
	_id = -1;
}

template <typename type_t>
Key<type_t>::Key(size_t id)
{
	_id = id;
}

template <typename type_t>
Key<type_t> Key<type_t>::createKey(size_t index)
{
	return (Key<type_t>(index));
}

template <typename type_t>
Key<type_t>::Key() :
_id(-1)
{

}