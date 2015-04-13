#pragma once

#include "MemoryPoolHelper.hpp"

namespace AGE
{
	namespace Physics
	{
		// Methods
		template <std::size_t Alignment, std::size_t ObjectNumberPerChunk>
		template <typename T>
		ObjectPool<T, Alignment, ObjectNumberPerChunk> &MemoryPoolHelper<Alignment, ObjectNumberPerChunk>::getMemoryPool(void)
		{
			static ObjectPool<T, Alignment, ObjectNumberPerChunk> MemoryPool;
			return MemoryPool;
		}

		template <std::size_t Alignment, std::size_t ObjectNumberPerChunk>
		template <typename T, typename... Args>
		T *MemoryPoolHelper<Alignment, ObjectNumberPerChunk>::create(Args &&...args)
		{
			return getMemoryPool<T>().create(std::forward<Args>(args)...);
		}

		template <std::size_t Alignment, std::size_t ObjectNumberPerChunk>
		template <typename T>
		void MemoryPoolHelper<Alignment, ObjectNumberPerChunk>::destroy(T *instance)
		{
			return getMemoryPool<T>().destroy(instance);
		}
	}
}