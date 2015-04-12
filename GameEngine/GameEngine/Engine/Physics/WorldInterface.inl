#pragma once

#include "WorldInterface.hpp"
#include "RigidBodyInterface.hpp"
#include "MaterialInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		inline WorldInterface::WorldInterface(PhysicsInterface *physics)
			: physics(physics)
		{
			return;
		}

		// Methods
		inline PhysicsInterface *WorldInterface::getPhysics(void)
		{
			return physics;
		}

		inline const PhysicsInterface *WorldInterface::getPhysics(void) const
		{
			return physics;
		}

		inline void WorldInterface::setGravity(float x, float y, float z)
		{
			setGravity(glm::vec3(x, y, z));
		}

		inline void WorldInterface::setTargetFPS(std::size_t target)
		{
			targetFPS = target;
		}

		inline std::size_t WorldInterface::getTargetFPS(void) const
		{
			return targetFPS;
		}

		inline void WorldInterface::update(float elapsedTime)
		{
			const float stepSize = 1.0f / static_cast<float>(targetFPS);
			accumulator += elapsedTime;
			while (accumulator >= stepSize)
			{
				simulate(stepSize);
				accumulator -= stepSize;
			}
		}

		inline void WorldInterface::destroyRigidBody(RigidBodyInterface *rigidBody)
		{
			delete rigidBody;
		}

		inline void WorldInterface::destroyMaterial(MaterialInterface *material)
		{
			delete material;
		}
	}
}