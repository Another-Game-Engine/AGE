#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace AGE
{
	namespace Physics
	{
		class WorldInterface;
		class MaterialInterface;

		class RigidBodyInterface
		{
			// Friendships
			friend WorldInterface;

		public:
			// Constructors
			RigidBodyInterface(void) = delete;

			RigidBodyInterface(WorldInterface *world);

			RigidBodyInterface(const RigidBodyInterface &) = delete;

			// Assignment Operators
			RigidBodyInterface &operator=(const RigidBodyInterface &) = delete;

			// Methods
			WorldInterface *getWorld(void);

			const WorldInterface *getWorld(void) const;

			MaterialInterface *getMaterial(void);

			const MaterialInterface *getMaterial(void) const;

			glm::vec3 getWorldCenterOfMass(void) const;

			// Virtual Methods
			virtual void setAngularDrag(float angularDrag) = 0;

			virtual float getAngularDrag(void) const = 0;

			virtual void setAngularVelocity(const glm::vec3 &angularVelocity) = 0;

			virtual glm::vec3 getAngularVelocity(void) const = 0;

			virtual void setCenterOfMass(const glm::vec3 &centerOfMass) = 0;

			virtual glm::vec3 getCenterOfMass(void) const = 0;

			virtual void setLinearDrag(float linearDrag) = 0;

			virtual float getLinearDrag(void) const = 0;

			virtual void setLinearVelocity(const glm::vec3 &linearVelocity) = 0;

			virtual glm::vec3 getLinearVelocity(void) const = 0;

			virtual void setMass(float mass) = 0;

			virtual float getMass(void) const = 0;

			virtual void setMaxAngularVelocity(float maxAngularVelocity) = 0;

			virtual float getMaxAngularVelocity(void) const = 0;

			virtual void setMaxDepenetrationVelocity(float maxDepenetrationVelocity) = 0;

			virtual float getMaxDepenetrationVelocity(void) const = 0;

			virtual void setPosition(const glm::vec3 &position) = 0;

			virtual glm::vec3 getPosition(void) const = 0;

			virtual void setRotation(const glm::quat &rotation) = 0;

			virtual glm::quat getRotation(void) const = 0;

		protected:
			// Destructor
			virtual ~RigidBodyInterface(void);

		private:
			// Attributes
			WorldInterface *world = nullptr;

			MaterialInterface *material = nullptr;
		};
	}
}

#include "RigidBodyInterface.inl"