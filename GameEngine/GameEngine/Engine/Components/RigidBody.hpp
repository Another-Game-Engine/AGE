#pragma once

#include <Components/Component.hh>
#include <Physics/RigidBodyInterface.hpp>
#include <Utils/Serialization/VectorSerialization.hpp>

namespace AGE
{
	struct Link;

	class RigidBody final : public ComponentBase
	{
		// Friendships
		friend Link;

	public:
		// Constructors
		RigidBody(void) = default;

		RigidBody(const RigidBody &) = delete;

		// Assignment Operators
		RigidBody &operator=(const RigidBody &) = delete;

		// Destructor
		~RigidBody(void) = default;

		// Methods
		void init(void);

		void setAngularDrag(float angularDrag);

		float getAngularDrag(void) const;

		void setAngularVelocity(const glm::vec3 &angularVelocity);

		glm::vec3 getAngularVelocity(void) const;

		void setCenterOfMass(const glm::vec3 &centerOfMass);

		glm::vec3 getCenterOfMass(void) const;

		void setLinearDrag(float linearDrag);

		float getLinearDrag(void) const;

		void setLinearVelocity(const glm::vec3 &linearVelocity);

		glm::vec3 getLinearVelocity(void) const;

		void setMass(float mass);

		float getMass(void) const;

		void setDiagonalInertiaTensor(const glm::vec3 &diagonalInertiaTensor);

		glm::vec3 getDiagonalInertiaTensor(void) const;

		void setMaxAngularVelocity(float maxAngularVelocity);

		float getMaxAngularVelocity(void) const;

		void setMaxDepenetrationVelocity(float maxDepenetrationVelocity);

		float getMaxDepenetrationVelocity(void) const;

		glm::vec3 getPosition(void) const;

		glm::quat getRotation(void) const;

		void affectByGravity(bool mustBeAffectedByGravity);

		bool isAffectedByGravity(void) const;

		void setAsKinematic(bool mustBeKinematic);

		bool isKinematic(void) const;

		void setCollisionDetectionMode(Physics::CollisionDetectionMode collisionDetectionMode);

		Physics::CollisionDetectionMode getCollisionDetectionMode(void) const;

		void addExplosionForce(float explosionForce, const glm::vec3 &explosionPosition, float explosionRadius, Physics::ForceMode forceMode = Physics::ForceMode::Force);

		void addForce(const glm::vec3 &force, Physics::ForceMode forceMode = Physics::ForceMode::Force);

		void addForceAtWorldPosition(const glm::vec3 &force, const glm::vec3 &position, Physics::ForceMode forceMode = Physics::ForceMode::Force);

		void addForceAtLocalPosition(const glm::vec3 &force, const glm::vec3 &position, Physics::ForceMode forceMode = Physics::ForceMode::Force);

		void addAbsoluteTorque(const glm::vec3 &torque, Physics::ForceMode forceMode = Physics::ForceMode::Force);
		
		void addRelativeTorque(const glm::vec3 &torque, Physics::ForceMode forceMode = Physics::ForceMode::Force);

		glm::vec3 getVelocityAtWorldPosition(const glm::vec3 &position) const;

		glm::vec3 getVelocityAtLocalPosition(const glm::vec3 &position) const;

		template <typename Archive>
		void save(Archive &ar, const std::uint32_t version) const;

		template <typename Archive>
		void load(Archive &ar, const std::uint32_t version);

	private:
		// Attributes
		Physics::RigidBodyInterface *rigidBody = nullptr;

		// Methods
		void setPosition(const glm::vec3 &position);

		void setRotation(const glm::quat &rotation);

		// Inherited Methods
		void reset(void);
	};
}

CEREAL_CLASS_VERSION(AGE::RigidBody, 1);

#include <Components/RigidBody.inl>