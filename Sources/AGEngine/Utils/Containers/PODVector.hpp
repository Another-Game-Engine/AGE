#pragma once

#include <Utils/Debug.hpp>
#include <atomic>

namespace AGE
{
	template <class T>
	class PODVector
	{
	public:
		PODVector()
			: _buffer(nullptr)
			, _capacity(0)
			, _index(0)
		{
			static_assert(std::is_pod<T>::value, "It's not a POD");
		}

		~PODVector()
		{
			if (_buffer)
			{
				free(_buffer);
			}
		}

		inline bool empty() const { return _index == 0; }
		inline std::size_t size() const { return _index; }

		void clear()
		{
			_index = 0;
		}

		void clearAndFree()
		{
			_index = 0;
			free(_buffer);
			_buffer = nullptr;
		}

		void push_back(const T &o)
		{
			if (_index >= _capacity)
			{
				_resize();
			}
			_buffer[_index++] = o;
		}

		void push_back(const T &&o)
		{
			if (_index >= _capacity)
			{
				_resize();
			}
			_buffer[_index++] = o;
		}

		T &copy_back()
		{
			if (_index >= _capacity)
			{
				_resize();
			}
			return _buffer[_index++];
		}

		T &operator[](std::size_t index)
		{
			AGE_ASSERT(index < _index);
			return _buffer[index];
		}

		const T &operator[](std::size_t index) const
		{
			AGE_ASSERT(index < _index);
			return _buffer[index];
		}

		T &back()
		{
			AGE_ASSERT(_index > 0);
			return _buffer[_index - 1];
		}

		const T &back() const
		{
			AGE_ASSERT(_index > 0);
			return _buffer[_index - 1];
		}

		const T *data() const
		{
			return _buffer;
		}

	private:
		void _resize()
		{
			if (_capacity == 0)
			{
				_capacity = 8;
			}
			else
			{
				_capacity *= 2;
			}
			if (_buffer == nullptr)
			{
				_buffer = (T*)malloc(sizeof(T) * _capacity);
			}
			else
			{
				_buffer = (T*)realloc((void*)_buffer, sizeof(T) * _capacity);
			}
		}

		T *_buffer;
		std::size_t _capacity;
		std::size_t _index;
	};
}