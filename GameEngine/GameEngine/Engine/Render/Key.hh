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
	bool isValid() const;
	void destroy();

private:
	size_t _id;
	explicit Key(size_t id);
};

template <typename type_t>
size_t Key<type_t>::getId() const
{
	return (_id);
}

template <typename type_t>
bool Key<type_t>::isValid () const
{
	return (_id != -1);
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