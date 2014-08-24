#pragma once

#include <cstdlib>
#include <new>
#include <utility>

namespace AGE
{
	namespace PtrQueueType
	{
		static uint16_t __counter = 0;
		struct Base
		{
			Base(std::uint16_t _uid = 0)
			: uid(_uid)
			{
			}
			virtual ~Base(){}
			const std::uint16_t uid;
		};
		template <typename T>
		struct BaseUid : public Base
		{
			static const std::uint16_t getId() { static uint16_t id = __counter++; return id; }
			BaseUid()
				: Base(BaseUid::getId())
			{}
			virtual ~BaseUid(){}
		};
	}

	template <typename Base = PtrQueueType::Base>
	class PtrQueue
	{
	public:
		PtrQueue(std::size_t chunkSize = 1024)
			: _chunkSize(chunkSize)
			, _data(nullptr)
			, _cursor(0)
			, _size(0)
			, _to(0)
		{
		}

		PtrQueue(PtrQueue &&o)
		{
			if (_data != nullptr)
				free(_data);
			_chunkSize = std::move(o._chunkSize);
			_data = std::move(o._data);
			_cursor = std::move(o._cursor);
			_size = std::move(o._size);
			_to = std::move(o._to);
			o._data = nullptr;
			o._size = 0;
		}

		PtrQueue&& operator=(PtrQueue &&o)
		{
			if (_data != nullptr)
				free(_data);
			_chunkSize = std::move(o._chunkSize);
			_data = std::move(o._data);
			_cursor = std::move(o._cursor);
			_size = std::move(o._size);
			_to = std::move(o._to);
			o._data = nullptr;
			o._size = 0;
			return std::move(*this);
		}

		~PtrQueue()
		{
			if (_data != nullptr)
				free(_data);
		}

		template <typename T>
		T* push()
		{
			if (_data == nullptr
				|| _size - _to < sizeof(T)+sizeof(std::size_t))
			{
				allocate<T>();
			}
			std::size_t s = sizeof(T);

			char *tmp = _data;
			tmp += _to;
			memcpy(tmp, &s, sizeof(std::size_t));
			tmp += sizeof(std::size_t);
			T* res = new(tmp)T();
			_to += sizeof(std::size_t) + sizeof(T);
			return res;
		}

		Base *pop()
		{
			if (isEmpty())
				return nullptr;

			char *tmp = _data;
			std::size_t soi = sizeof(std::size_t);

			tmp += _cursor;
			std::size_t s = *reinterpret_cast<std::size_t*>(tmp);
			_cursor += s + soi;
			tmp += soi;
			return ((Base*)(tmp));
		}

		void clear()
		{
			_cursor = _to = 0;
		}

		void release()
		{
			clear();
			if (_data != nullptr)
				free(_data);
		}

		bool isEmpty()
		{
			if (_cursor >= _to)
			{
				_cursor = 0;
				_to = 0;
				return true;
			}
			return false;
		}
	private:
		template <typename T>
		void allocate()
		{
			std::size_t sizeOfType = sizeof(T);
			
			while (_size - _to <= sizeOfType + sizeof(std::size_t)
				|| _size <= _to)
			{
				_size += _chunkSize;
				_data = (char*)(realloc(_data, _size));
			}
		}
	private:
		std::size_t _chunkSize;
		char *_data;
		std::size_t _cursor;
		std::size_t _size;
		std::size_t _to;

		PtrQueue(PtrQueue &o);
		PtrQueue& operator=(PtrQueue &o);
	};
}