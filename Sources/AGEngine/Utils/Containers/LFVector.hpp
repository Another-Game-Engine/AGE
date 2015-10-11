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
		inline void         push(const T &object)
		{
			std::size_t index = _index.fetch_add(1);
			AGE_ASSERT(index < _capacity);
			_array[index] = object;
		}
		inline bool         pushChunk(const T *data, std::size_t size)
		{
			std::size_t index = _index.fetch_add(size);
			if (index >= _capacity)
			{
				_index = _capacity;
				return false;
			}
			memcpy(_array + index, data, size * sizeof(T));
			return true;
		}
		template <typename ...Args>
		inline void         emplace(Args... args)
		{
			std::size_t index = _index.fetch_add(1);
			AGE_ASSERT(index < _capacity);
			_array[index] = T(args...);
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