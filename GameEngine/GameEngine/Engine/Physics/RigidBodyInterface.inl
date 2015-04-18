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
		// Static Methods
		inline float RigidBodyInterface::GetDefaultAngularDrag(void)
		{
			return 0.05f;
		}

		inline glm::vec3 RigidBodyInterface::GetDefaultAngularVelocity(void)
		{
			return glm::vec3();
		}

		inline glm::vec3 RigidBodyInterface::GetDefaultCenterOfMass(void)
		{
			return glm::vec3();
		}

		inline float RigidBodyInterface::GetDefaultLinearDrag(void)
		{
			return 0.0f;
		}

		inline glm::vec3 RigidBodyInterface::GetDefaultLinearVelocity(void)
		{
			return glm::vec3();
		}

		inline float RigidBodyInterface::GetDefaultMass(void)
		{
			return 1.0f;
		}

		inline glm::vec3 RigidBodyInterface::GetDefaultDiagonalInertiaTensor(void)
		{
			return glm::vec3();
		}

		inline float RigidBodyInterface::GetDefaultMaxAngularVelocity(void)
		{
			return std::numeric_limits<float>::max();
		}

		inline float RigidBodyInterface::GetDefaultMaxDepenetrationVelocity(void)
		{
			return std::numeric_limits<float>::max();
		}

		inline bool RigidBodyInterface::IsAffectedByGravityByDefault(void)
		{
			return true;
		}

		inline bool RigidBodyInterface::IsKinematicByDefault(void)
		{
			return false;
		}

		inline CollisionDetectionMode RigidBodyInterface::GetDefaultCollisionDetectionMode(void)
		{
			return CollisionDetectionMode::Continuous;
		}

		// Constructors
		inline RigidBodyInterface::RigidBodyInterface(WorldInterface *world, Private::GenericData *data)
			: world(world), data(data)
		{
			assert(data != nullptr && "Invalid data");
		}

		// Destructor
		inline RigidBodyInterface::~RigidBodyInterface(void)
		{
			rigidBody = nullptr;
			world = nullptr;
		}

		// Methods
		inline NewRigidBody *RigidBodyInterface::getRigidBody(void)
		{
			assert(rigidBody && "Invalid rigidBody");
			return rigidBody;
		}

		inline const NewRigidBody *RigidBodyInterface::getRigidBody(void) const
		{
			assert(rigidBody && "Invalid rigidBody");
			return rigidBody;
		}

		inline WorldInterface *RigidBodyInterface::getWorld(void)
		{
			return world;
		}

		inline const WorldInterface *RigidBodyInterface::getWorld(void) const
		{
			return world;
		}

		inline Private::GenericData *RigidBodyInterface::getData(void)
		{
			return data;
		}

		inline const Private::GenericData *RigidBodyInterface::getData(void) const
		{
			return data;
		}

		template <typename T>
		inline T *RigidBodyInterface::getDataAs(void)
		{
			return static_cast<T *>(getData()->data);
		}

		template <typename T>
		inline const T *RigidBodyInterface::getDataAs(void) const
		{
			return static_cast<const T *>(getData()->data);
		}

		inline glm::vec3 RigidBodyInterface::getWorldCenterOfMass(void) const
		{
			return getPosition() + getCenterOfMass();
		}

		inline void RigidBodyInterface::addExplosionForce(float explosionForce, const glm::vec3 &explosionPosition, float explosionRadius, ForceMode forceMode)
		{
			assert(explosionRadius > 0.0f && "Invalid explosion radius");
			const float force = explosionForce <= 0.0f ? std::numeric_limits<float>::max() : explosionForce;
			const glm::vec3 position = getPosition();
			const float distance = glm::distance(explosionPosition, position);
			float ratio = std::max(1.0f - distance / explosionRadius, 0.0f);
			addForce(glm::normalize(getPosition() - explosionPosition) * force * ratio, forceMode);
		}
	}
}