#pragma once

#include "../../../GameEngine/Engine/Utils/ObjectPool.hpp"

namespace AGE
{
	namespace Physics
	{
		template <std::size_t Alignment = 16, std::size_t ObjectNumberPerChunk = 1024>
		class MemoryPoolHelper
		{
		public:
			// Constructors
			MemoryPoolHelper(void) = default;

			MemoryPoolHelper(const MemoryPoolHelper &) = delete;

			// Assignment Operators
			MemoryPoolHelper &operator=(const MemoryPoolHelper &) = delete;

			// Destructor
			virtual ~MemoryPoolHelper(void) = default;

			// Methods
			template <typename T, typename... Args>
			T *create(Args &&...args);

			template <typename T>
			void destroy(T *instance);

		private:
			// Methods
			template <typename T>
			ObjectPool<T, Alignment, ObjectNumberPerChunk> &getMemoryPool(void);
		};
	}
}

#include "MemoryPoolHelper.inl"