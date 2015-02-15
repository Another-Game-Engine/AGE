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
	static Key<TYPE> createKey(int index);

public:
	int getId() const;
	operator bool() const;
	void destroy();

private:
	int _id;
	explicit Key(int id);
};

template <typename type_t>
int Key<type_t>::getId() const
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
Key<type_t>::Key(int id)
{
	_id = id;
}

template <typename type_t>
Key<type_t> Key<type_t>::createKey(int index)
{
	return (Key<type_t>(index));
}

template <typename type_t>
Key<type_t>::Key() :
_id(-1)
{

}