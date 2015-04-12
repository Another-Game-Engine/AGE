#pragma once

#include "WorldInterface.hpp"
#include "MaterialInterface.hpp"
#include "BoxColliderInterface.hpp"
#include "SphereColliderInterface.hpp"
#include "CapsuleColliderInterface.hpp"
#include "MeshColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		inline ColliderInterface::ColliderInterface(WorldInterface *world)
			: world(world), material(world->createMaterial(this))
		{
			assert(world != nullptr && "Invalid world");
			assert(material != nullptr && "Invalid material");
		}

		// Destructor
		inline ColliderInterface::~ColliderInterface(void)
		{
			world->destroyMaterial(material);
			material = nullptr;
			world = nullptr;
		}

		// Methods
		inline WorldInterface *ColliderInterface::getWorld(void)
		{
			return world;
		}

		inline const WorldInterface *ColliderInterface::getWorld(void) const
		{
			return world;
		}

		inline MaterialInterface *ColliderInterface::getMaterial(void)
		{
			return material;
		}

		inline const MaterialInterface *ColliderInterface::getMaterial(void) const
		{
			return material;
		}

		template <ColliderType Type>
		inline bool ColliderInterface::is(void) const
		{
			return is(Type);
		}

		inline bool ColliderInterface::is(ColliderType colliderType) const
		{
			return getColliderType() == colliderType;
		}

		template <ColliderType Type>
		inline typename ColliderInterface::DeduceColliderType<Type>::Type *ColliderInterface::as(void)
		{
			return is<Type>() ? static_cast<DeduceColliderType<Type>::Type *>(this) : nullptr;
		}

		template <ColliderType Type>
		inline const typename ColliderInterface::DeduceColliderType<Type>::Type *ColliderInterface::as(void) const
		{
			return is<Type>() ? static_cast<const DeduceColliderType<Type>::Type *>(this) : nullptr;
		}
	}
}