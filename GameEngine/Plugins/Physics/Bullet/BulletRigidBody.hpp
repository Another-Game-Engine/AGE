#pragma once

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include "RigidBodyInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class BulletWorld;
		class BulletCollider;

		class BulletRigidBody final : public RigidBodyInterface
		{
			// Friendships
			friend ObjectPool < BulletRigidBody >;

			friend BulletCollider;

		public:
			// Constructors
			BulletRigidBody(void) = delete;

			BulletRigidBody(BulletWorld *world, Private::GenericData *data);

			BulletRigidBody(const BulletRigidBody &) = delete;

			// Assignment Operators
			BulletRigidBody &operator=(const BulletRigidBody &) = delete;

		private:
			// Static Attributes
			static btEmptyShape EmptyShape;

			// Attributes
			float maxAngularVelocity = 0.0f;

			float maxDepenetrationVelocity = 0.0f;

			// Destructor
			~BulletRigidBody(void);

			// Inherited Methods
			void setAngularDrag(float angularDrag) override final;

			float getAngularDrag(void) const override final;

			void setAngularVelocity(const glm::vec3 &angularVelocity) override final;

			glm::vec3 getAngularVelocity(void) const override final;

			void setCenterOfMass(const glm::vec3 &centerOfMass) override final;

			glm::vec3 getCenterOfMass(void) const override final;

			void setLinearDrag(float linearDrag) override final;

			float getLinearDrag(void) const override final;

			void setLinearVelocity(const glm::vec3 &linearVelocity) override final;

			glm::vec3 getLinearVelocity(void) const override final;

			void setMass(float mass) override final;

			float getMass(void) const override final;

			void setDiagonalInertiaTensor(const glm::vec3 &diagonalInertiaTensor) override final;

			glm::vec3 getDiagonalInertiaTensor(void) const override final;

			void setMaxAngularVelocity(float maxAngularVelocity) override final;

			double getMaxAngularVelocity(void) const override final;

			void setMaxDepenetrationVelocity(float maxDepenetrationVelocity) override final;

			float getMaxDepenetrationVelocity(void) const override final;

			void setPosition(const glm::vec3 &position) override final;

			glm::vec3 getPosition(void) const override final;

			void setRotation(const glm::quat &rotation) override final;

			glm::quat getRotation(void) const override final;

			void affectByGravity(bool mustBeAffectedByGravity) override final;

			bool isAffectedByGravity(void) const override final;

			void setAsKinematic(bool mustBeKinematic) override final;

			bool isKinematic(void) const override final;

			void setCollisionDetectionMode(CollisionDetectionMode collisionDetectionMode) override final;

			CollisionDetectionMode getCollisionDetectionMode(void) const override final;

			void addForce(const glm::vec3 &force, ForceMode forceMode) override final;

			void addForceAtWorldPosition(const glm::vec3 &force, const glm::vec3 &position, ForceMode forceMode) override final;

			void addForceAtLocalPosition(const glm::vec3 &force, const glm::vec3 &position, ForceMode forceMode) override final;

			void addAbsoluteTorque(const glm::vec3 &torque, ForceMode forceMode) override final;

			void addRelativeTorque(const glm::vec3 &torque, ForceMode forceMode) override final;

			glm::vec3 getVelocityAtWorldPosition(const glm::vec3 &position) const override final;

			glm::vec3 getVelocityAtLocalPosition(const glm::vec3 &position) const override final;
		};
	}
}