#pragma once

#include <string>

#include <glm/glm.hpp>

#include "../Utils/Dependency.hpp"
#include "FilterGroup.hpp"

namespace AGE
{
	namespace Physics
	{
		class PhysicsInterface;
		class RigidBodyInterface;
		class MaterialInterface;

		class WorldInterface : public Dependency < WorldInterface >
		{
			// Friendship
			friend RigidBodyInterface;

		public:
			WorldInterface(void) = delete;

			// Constructors
			WorldInterface(PhysicsInterface *physics);

			WorldInterface(const WorldInterface &) = delete;

			// Assignment Operators
			WorldInterface &operator=(const WorldInterface &) = delete;

			// Methods
			PhysicsInterface *getPhysics(void);

			const PhysicsInterface *getPhysics(void) const;

			void setGravity(float x, float y, float z);

			void setTargetFPS(std::size_t target);

			std::size_t getTargetFPS(void) const;

			void update(float elapsedTime);

			void destroyRigidBody(RigidBodyInterface *rigidBody);

			// Virtual Methods
			virtual void setGravity(const glm::vec3 &gravity) = 0;

			virtual glm::vec3 getGravity(void) const = 0;

			virtual void enableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2) = 0;

			virtual void disableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2) = 0;

			virtual RigidBodyInterface *createRigidBody(const glm::vec3 &position) = 0;

		protected:
			// Destructor
			virtual ~WorldInterface(void) = default;

		private:
			// Attributes
			PhysicsInterface *physics = nullptr;

			std::size_t targetFPS = 60;

			float accumulator = 0.0f;

			// Methods
			void destroyMaterial(MaterialInterface *material);

			// Virtual Methods
			virtual MaterialInterface *createMaterial(void) = 0;

			virtual void simulate(float stepSize) = 0;
		};
	}
}

#include "WorldInterface.inl"
