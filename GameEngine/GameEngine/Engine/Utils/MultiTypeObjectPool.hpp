#pragma once

#include "ObjectPool.hpp"
#include "UniqueTypeId.hpp"
#include <map>

namespace AGE
{
	template <std::size_t Alignement = 16, std::size_t ObjectNumberPerChunk = 1024>
	class MultiTypeObjectPool
	{
	public:
		template <typename T, typename ...Args>
		T *create(Args &&...args)
		{
			auto id = TypeID::Get<T>();
			if (_pools.find(id) == std::end(_pools))
			{
				_pools.insert(std::make_pair(id, new ObjectPool<T, Alignement, ObjectNumberPerChunk>()));
			}
			return ((ObjectPool<T, Alignement, ObjectNumberPerChunk>*)(_pools[id]))->create(args...);
		}

		void destroy(void *ptr, UniqueTypeId typeId)
		{
			if (_pools.find(typeId) == std::end(_pools))
			{
				assert(false && "Unknown type");
			}
			_pools[typeId]->destroy(ptr);
		}

		template <typename T>
		void destroy(T *ptr)
		{
			auto typeId = TypeID::Get<T>();
			if (_pools.find(typeId) == std::end(_pools))
			{
				assert(false && "Unknown type");
			}
			((ObjectPool<T, Alignement, ObjectNumberPerChunk>*)(_pools[typeId]))->destroy(ptr);
		}
	private:
		std::map<UniqueTypeId, IObjectPool*> _pools;
	};
}