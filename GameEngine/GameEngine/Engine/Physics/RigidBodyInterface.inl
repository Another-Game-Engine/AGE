#pragma once

#include "RigidBodyInterface.hpp"
#include "WorldInterface.hpp"
#include "MaterialInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		inline RigidBodyInterface::RigidBodyInterface(WorldInterface *world)
			: world(world), material(world->createMaterial())
		{
			assert(material != nullptr && "Impossible to create material");
			material->rigidBody = this;
		}

		// Destructor
		inline RigidBodyInterface::~RigidBodyInterface(void)
		{
			world->destroyMaterial(material);
			material = nullptr;
			world = nullptr;
		}

		// Methods
		inline WorldInterface *RigidBodyInterface::getWorld(void)
		{
			return world;
		}

		inline const WorldInterface *RigidBodyInterface::getWorld(void) const
		{
			return world;
		}

		inline MaterialInterface *RigidBodyInterface::getMaterial(void)
		{
			return material;
		}

		inline const MaterialInterface *RigidBodyInterface::getMaterial(void) const
		{
			return material;
		}

		inline glm::vec3 RigidBodyInterface::getWorldCenterOfMass(void) const
		{
			return getPosition() + getCenterOfMass();
		}
	}
}