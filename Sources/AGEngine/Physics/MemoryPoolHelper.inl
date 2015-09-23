#pragma once

#include "MemoryPoolHelper.hpp"

namespace AGE
{
	namespace Physics
	{
		// Methods
		template <class RigidBodyType, class MaterialType, class BoxColliderType, class CapsuleColliderType, class MeshColliderType, class SphereColliderType, std::size_t Alignment, std::size_t ObjectNumberPerChunk>
		template <typename T, typename... Args, class Enable>
		inline T *MemoryPoolHelper<RigidBodyType, MaterialType, BoxColliderType, CapsuleColliderType, MeshColliderType, SphereColliderType, Alignment, ObjectNumberPerChunk>::create(Args &&...args)
		{
			return PoolHelper<T>(*this).create(std::forward<Args>(args)...);
		}

		template <class RigidBodyType, class MaterialType, class BoxColliderType, class CapsuleColliderType, class MeshColliderType, class SphereColliderType, std::size_t Alignment, std::size_t ObjectNumberPerChunk>
		template <typename T, class Enable>
		inline void MemoryPoolHelper<RigidBodyType, MaterialType, BoxColliderType, CapsuleColliderType, MeshColliderType, SphereColliderType, Alignment, ObjectNumberPerChunk>::destroy(T *instance)
		{
			PoolHelper<T>(*this).destroy(instance);
		}
	}
}