#pragma once

#include <utility>

#include "BufferPool.hpp"

namespace AGE
{
	template <typename T, std::size_t Alignement = 16, std::size_t ObjectNumberPerChunk = 1024>
	class ObjectPool final : public BufferPool
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
		void *allocateObject() override final
		{
			SCOPE_profile_cpu_function("Memory");

			void *res;
			const bool error = _allocateObject(res);
			assert(error);
			return res;
		}

		template <typename ...Args>
		T *create(Args &&...args)
		{
			SCOPE_profile_cpu_function("Memory");

			return new (allocateObject()) T(std::forward<Args>(args)...);
		}

		void destroy(void *ptr) override final
		{
			SCOPE_profile_cpu_function("Memory");

			destroy(static_cast<T *>(ptr));
		}

		void destroy(T *ptr)
		{
			SCOPE_profile_cpu_function("Memory");

			ptr->~T();
			_dealocateObject(ptr);
		}
	};
}