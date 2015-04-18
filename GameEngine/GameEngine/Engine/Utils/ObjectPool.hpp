#pragma once

#include <utility>

#include "BufferPool.hpp"

namespace AGE
{
	class IObjectPool
	{
	public:
		virtual void destroy(void *ptr) = 0;
		virtual void *allocateObject() = 0;
	};

	template <typename T, std::size_t Alignement = 16, std::size_t ObjectNumberPerChunk = 1024>
	class ObjectPool : public BufferPool, public IObjectPool
	{
	public:
		ObjectPool()
		{
			const bool init = _init(sizeof(T), Alignement, ObjectNumberPerChunk);
			assert(init);
		}
		virtual ~ObjectPool()
		{
			//assert(_objectNumber == 0 && "All object has not been destroyed ! Memory Leak !");
		}

		// Allocate object space but do not call new()
		virtual void *allocateObject()
		{
			void *res;
			const bool error = _allocateObject(res);
			assert(error);
			return res;
		}

		template <typename ...Args>
		T *create(Args &&...args)
		{
			return new (allocateObject()) T(std::forward<Args>(args)...);
		}

		virtual void destroy(void *ptr)
		{
			destroy(static_cast<T *>(ptr));
		}

		void destroy(T *ptr)
		{
			ptr->~T();
			_dealocateObject(ptr);
		}
	};
}