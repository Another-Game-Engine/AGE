#pragma once

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
			bool init = _init(sizeof(T), Alignement, ObjectNumberPerChunk);
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
			auto error = _allocateObject(res);
			assert(error);
			return res;
		}

		template <typename ...Args>
		T *create(Args &&...args)
		{
			void *res = allocateObject();
			T *tRes = new (res)T(args...);
			return tRes;
		}

		virtual void destroy(void *ptr)
		{
			destroy((T*)ptr);
		}

		void destroy(T *ptr)
		{
			ptr->~T();
			_dealocateObject(ptr);
		}
	};
}