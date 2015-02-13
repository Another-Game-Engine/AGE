#pragma once

#include "BufferPool.hpp"

namespace AGE
{
	class IObjectPool
	{
	public:
		virtual void destroy(void *ptr) = 0;
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
		}

		T *create()
		{
			void *res;
			_allocateObject(res);
			T *tRes = new (res)T();
			return tRes;
		}

		template <typename ...Args>
		T *create(Args &&...args)
		{
			void *res;
			_allocateObject(res);
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