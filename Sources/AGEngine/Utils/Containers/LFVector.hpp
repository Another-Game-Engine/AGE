#pragma once

#include <Utils/Debug.hpp>
#include <atomic>

namespace AGE
{
	template <class T, size_t Size>
	class LFVector
	{
	public:
		LFVector()
			: _capacity(Size)
		{
			memset(_array, 0, sizeof(T) * Size);
			_index = 0;
		}

		LFVector(const LFVector &o) = delete;
		LFVector(LFVector &&o) = delete;
		LFVector &operator=(const LFVector &) = delete;
		LFVector &operator=(LFVector &&) = delete;

		inline size_t const capacity() const { return _capacity; }
		inline size_t const size() const { return _index; }
		inline void         clear() { _index = 0; }
		inline bool         push(const T &object)
		{
			std::size_t index = _index.fetch_add(1);
			if (index >= _capacity)
			{
				_index = _capacity;
				return false;
			}
			_array[index] = object;
			return true;
		}
		inline std::size_t         pushChunk(const T *data, std::size_t size)
		{
			std::size_t index = _index.fetch_add(size);
			std::size_t availableSize = size;

			if (index >= _capacity)
			{
				_index = _capacity;
				return 0;
			}
			if (index + size >= _capacity)
			{
				_index = _capacity;
				availableSize = _capacity - index;
			}
			memcpy(_array + index, data, availableSize * sizeof(T));
			return availableSize;
		}
		template <typename ...Args>
		inline bool         emplace(Args... args)
		{
			std::size_t index = _index.fetch_add(1);
			if (index >= _capacity)
			{
				_index = _capacity;
				return false;
			}
			_array[index] = T(args...);
			return true;
		}
		T &operator[](const size_t i)
		{
			AGE_ASSERT(i < _index);
			return _array[i];
		}
		const T &operator[](const size_t i) const
		{
			AGE_ASSERT(i < _index);
			return _array[i];
		}
		T &back()
		{
			std::size_t index = _index.load();
			AGE_ASSERT(index > 0);
			return _array[index - 1];
		}
		const T &back() const
		{
			std::size_t index = _index.load();
			AGE_ASSERT(index > 0);
			return _array[index - 1];
		}
		inline T* data()
		{
			return &(_array[0]);
		}

		inline const T* data() const
		{
			return &(_array[0]);
		}
	private:
		const size_t             _capacity;
		std::atomic_size_t       _index;
		T                        _array[Size];
	};
}