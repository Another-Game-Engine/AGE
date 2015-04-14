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
		inline ColliderInterface::ColliderInterface(WorldInterface *world, Private::GenericData *data)
			: world(world), data(data), material(world->createMaterial(this))
		{
			assert(world != nullptr && "Invalid world");
			assert(data != nullptr && "Invalid data");
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

		inline Private::GenericData *ColliderInterface::getData(void)
		{
			return data;
		}

		inline const Private::GenericData *ColliderInterface::getData(void) const
		{
			return data;
		}

		template <typename T>
		inline T *ColliderInterface::getDataAs(void)
		{
			return static_cast<T *>(getData()->data);
		}

		template <typename T>
		inline const T *ColliderInterface::getDataAs(void) const
		{
			return static_cast<const T *>(getData()->data);
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
			assert(is<Type>() && "Invalid type");
			return dynamic_cast<DeduceColliderType<Type>::Type *>(this);
		}

		template <ColliderType Type>
		inline const typename ColliderInterface::DeduceColliderType<Type>::Type *ColliderInterface::as(void) const
		{
			assert(is<Type>() && "Invalid type");
			return dynamic_cast<DeduceColliderType<Type>::Type *>(this);
		}
	}
}