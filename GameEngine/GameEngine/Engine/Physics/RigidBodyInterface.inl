#pragma once

#include <limits>
#include <algorithm>

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

		inline void RigidBodyInterface::addExplosionForce(float explosionForce, const glm::vec3 &explosionPosition, float explosionRadius, ForceMode forceMode)
		{
			const float force = explosionForce <= 0.0f ? std::numeric_limits<float>::max() : explosionForce;
			const glm::vec3 position = getPosition();
			const float distance = glm::distance(explosionPosition, position);
			float ratio = 0.0f;
			if (distance <= std::numeric_limits<float>::epsilon())
			{
				ratio = 1.0f;
			}
			else
			{
				ratio = std::max(1.0f - distance / explosionRadius, 0.0f);
			}
			addForce(glm::normalize(getPosition() - explosionPosition) * explosionForce * ratio, forceMode);
		}
	}
}