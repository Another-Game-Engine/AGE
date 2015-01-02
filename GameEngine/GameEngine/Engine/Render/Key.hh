#pragma once

#include <iostream>
#include <queue>
#include <cstdint>
#include <Utils/Containers/Vector.hpp>
#include <assert.h>

namespace gl
{
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
				_data.push_back(InternalData());
			}
			assert(_data.size() > index);
			auto &data = _data[index];
			if (!data.trash.empty())
			{
				auto res = Key<TYPE>(data.trash.front(), index);
				data.trash.pop();
				return res;
			}
			return (Key<TYPE>(data.id++, index));
		}

		static Key<TYPE> createKeyWithId(std::size_t _id, std::size_t index = 0)
		{
			return (Key<TYPE>(_id, index));
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
		size_t _index;
		explicit Key(size_t id, size_t index);
		static AGE::Vector<InternalData> _data;
	};
}

#include <Render/Key.hpp>
