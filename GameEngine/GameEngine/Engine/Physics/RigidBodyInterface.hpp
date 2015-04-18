#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "ForceMode.hpp"
#include "CollisionDetectionMode.hpp"
#include "FilterGroup.hpp"
#include "GenericData.hpp"

namespace AGE
{
	class NewRigidBody;

	namespace Physics
	{
		class WorldInterface;
		class MaterialInterface;

		class RigidBodyInterface
		{
			// Friendships
			friend NewRigidBody;

		public:
			// Constructors
			RigidBodyInterface(void) = delete;

			RigidBodyInterface(WorldInterface *world, Private::GenericData *data);

			RigidBodyInterface(const RigidBodyInterface &) = delete;

			// Assignment Operators
			RigidBodyInterface &operator=(const RigidBodyInterface &) = delete;

			// Methods
			NewRigidBody *getRigidBody(void);

			const NewRigidBody *getRigidBody(void) const;

			WorldInterface *getWorld(void);

			const WorldInterface *getWorld(void) const;

			Private::GenericData *getData(void);

			const Private::GenericData *getData(void) const;

			template <typename T>
			T *getDataAs(void);

			template <typename T>
			const T *getDataAs(void) const;

			glm::vec3 getWorldCenterOfMass(void) const;

			void addExplosionForce(float explosionForce, const glm::vec3 &explosionPosition, float explosionRadius, ForceMode forceMode);

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

			virtual void setDiagonalInertiaTensor(const glm::vec3 &diagonalInertiaTensor) = 0;

			virtual glm::vec3 getDiagonalInertiaTensor(void) const = 0;

			virtual void setMaxAngularVelocity(float maxAngularVelocity) = 0;

			virtual float getMaxAngularVelocity(void) const = 0;

			virtual void setMaxDepenetrationVelocity(float maxDepenetrationVelocity) = 0;

			virtual float getMaxDepenetrationVelocity(void) const = 0;

			virtual void setPosition(const glm::vec3 &position) = 0;

			virtual glm::vec3 getPosition(void) const = 0;

			virtual void setRotation(const glm::quat &rotation) = 0;

			virtual glm::quat getRotation(void) const = 0;

			virtual void affectByGravity(bool mustBeAffectedByGravity) = 0;

			virtual bool isAffectedByGravity(void) const = 0;

			virtual void setAsKinematic(bool mustBeKinematic) = 0;

			virtual bool isKinematic(void) const = 0;

			virtual void setCollisionDetectionMode(CollisionDetectionMode collisionDetectionMode) = 0;

			virtual CollisionDetectionMode getCollisionDetectionMode(void) const = 0;

			virtual void addForce(const glm::vec3 &force, ForceMode forceMode) = 0;

			virtual void addForceAtWorldPosition(const glm::vec3 &force, const glm::vec3 &position, ForceMode forceMode) = 0;

			virtual void addForceAtLocalPosition(const glm::vec3 &force, const glm::vec3 &position, ForceMode forceMode) = 0;

			virtual void addAbsoluteTorque(const glm::vec3 &torque, ForceMode forceMode) = 0;

			virtual void addRelativeTorque(const glm::vec3 &torque, ForceMode forceMode) = 0;

			virtual glm::vec3 getVelocityAtWorldPosition(const glm::vec3 &position) const = 0;

			virtual glm::vec3 getVelocityAtLocalPosition(const glm::vec3 &position) const = 0;

		protected:
			// Static Methods
			static float GetDefaultAngularDrag(void);

			static glm::vec3 GetDefaultAngularVelocity(void);

			static glm::vec3 GetDefaultCenterOfMass(void);

			static float GetDefaultLinearDrag(void);

			static glm::vec3 GetDefaultLinearVelocity(void);

			static float GetDefaultMass(void);

			static glm::vec3 GetDefaultDiagonalInertiaTensor(void);

			static float GetDefaultMaxAngularVelocity(void);

			static float GetDefaultMaxDepenetrationVelocity(void);

			static bool IsAffectedByGravityByDefault(void);

			static bool IsKinematicByDefault(void);

			static CollisionDetectionMode GetDefaultCollisionDetectionMode(void);

			// Destructor
			virtual ~RigidBodyInterface(void);

		private:
			// Attributes
			NewRigidBody *rigidBody = nullptr;

			WorldInterface *world = nullptr;

			Private::GenericData *data;
		};
	}
}

#include "RigidBodyInterface.inl"