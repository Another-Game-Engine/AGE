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
	static Key<TYPE> createKey(size_t index = 0)
	{
		if (_data.size() <= index)
		{
			_data.resize(index + 1);
		}
		auto &data = _data[index];
		if (!data.trash.empty())
		{
			auto res = Key<TYPE>(data.trash.front());
			data.trash.pop();
			return res;
		}
		return (Key<TYPE>(data.id++));
	}

	static Key<TYPE> createKeyWithId(std::size_t _id)
	{
		return (Key<TYPE>(_id));
	}

	Key();
	~Key();
	Key(Key<TYPE> const &copy);
	Key<TYPE> &operator=(Key<TYPE> const &t);
	size_t getId() const;
	bool empty() const;
	bool operator!() const;
	bool operator==(Key<TYPE> const &compare) const;
	bool operator!=(Key<TYPE> const &compare) const;
	bool operator<(Key<TYPE> const &compare) const;
	bool operator>(Key<TYPE> const &compare) const;
	void destroy();

private:
	size_t _id;
	explicit Key(size_t id);
	static AGE::Vector<InternalData> _data;
};

#include <Render/Key.hpp>
