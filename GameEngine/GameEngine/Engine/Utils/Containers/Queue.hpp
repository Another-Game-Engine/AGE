#pragma once

#include <crtdefs.h>
#include <cassert>

namespace AGE
{
	template <class T>
	class Queue
	{
	public:
		Queue()
			: _size(0)
			, _start(0)
			, _end(0)
			, _chunkSize(124)
		{}

		~Queue()
		{
			if (_data)
				delete[] _data;
		}

		Queue(const Queue &o)
		{}

		Queue(Queue &&o)
		{}

		Queue &operator=(const Queue &o)
		{
			return *this;
		}

		std::size_t size() const
		{
			return _end;
		}

		bool empty() const
		{
			return _size == 0 || _end == 0 || _end == _start;
		}

		const T &front() const
		{
			assert(!empty());
			return _data[_start];
		}

		T &front()
		{
			assert(!empty());
			return _data[_start];
		}

		const T &back() const
		{
			assert(!empty());
			return _data[_end - 1];
		}

		T &back()
		{
			assert(!empty());
			return _data[_end - 1];
		}

		void push(const T &e)
		{
			if (_end == _size)
				_allocate();
			_data[_end] = e;
			++_end;
		}

		void push(T &&e)
		{
			if (_end == _size)
				_allocate();
			_data[_end] = e;
			++_end;
		}

		template <typename... Args>
		void emplace(Args &&...args)
		{
			if (_end == _size)
				_allocate();
			_data[_end] = T(args...);
			++_end;
		}

		void pop()
		{
			if (empty())
				return;
			++_start;
			if (_start == _end)
			{
				clear();
			}
		}

		void clear()
		{
			_start = 0;
			_end = 0;
		}

	private:
		void _allocate()
		{
			if (_size == 0)
			{
				_data = new T[_chunkSize]();
				assert(_data != nullptr);
				_size = _chunkSize;
			}
			else
			{
				_size += _chunkSize;
				auto tmp = new T[_size]();
				assert(tmp != nullptr);
				for (std::size_t i = _start; i < _end; ++i)
				{
					tmp[i] = _data[i];
				}
				delete[] _data;
				_data = tmp;
			}
		}

		std::size_t _size;
		std::size_t _start;
		std::size_t _end;
		std::size_t _chunkSize;
		T *_data;
	};
}