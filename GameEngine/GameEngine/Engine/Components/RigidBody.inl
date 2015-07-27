#pragma once

#include <Components/RigidBody.hpp>

namespace AGE
{
	// Methods
	template <typename Archive>
	inline void RigidBody::save(Archive &ar, const std::uint32_t version) const
	{
		ar(cereal::make_nvp("AngularDrag", getAngularDrag()));
		ar(cereal::make_nvp("AngularVelocity", getAngularVelocity()));
		ar(cereal::make_nvp("CenterOfMass", getCenterOfMass()));
		ar(cereal::make_nvp("LinearDrag", getLinearDrag()));
		ar(cereal::make_nvp("LinearVelocity", getLinearVelocity()));
		ar(cereal::make_nvp("Mass", getMass()));
		ar(cereal::make_nvp("DiagonalInertiaTensor", getDiagonalInertiaTensor()));
		ar(cereal::make_nvp("MaxAngularVelocity", getMaxAngularVelocity()));
		ar(cereal::make_nvp("MaxDepenetrationVelocity", getMaxDepenetrationVelocity()));
		ar(cereal::make_nvp("IsAffectedByGravity", isAffectedByGravity()));
		ar(cereal::make_nvp("IsKinematic", isKinematic()));
		ar(cereal::make_nvp("CollisionDetectionMode", static_cast<std::uint8_t>(getCollisionDetectionMode())));
	}

	template <typename Archive>
	inline void RigidBody::load(Archive &ar, const std::uint32_t version)
	{
		init();
		float angularDrag;
		glm::vec3 angularVelocity;
		glm::vec3 centerOfMass;
		float linearDrag;
		glm::vec3 linearVelocity;
		float mass;
		glm::vec3 diagonalInertiaTensor;
		float maxAngularVelocity;
		float maxDepenetrationVelocity;
		bool isAffectedByGravity;
		bool isKinematic;
		std::uint8_t collisionDetectionMode;
		ar(cereal::make_nvp("AngularDrag", angularDrag));
		ar(cereal::make_nvp("AngularVelocity", angularVelocity));
		ar(cereal::make_nvp("CenterOfMass", centerOfMass));
		ar(cereal::make_nvp("LinearDrag", linearDrag));
		ar(cereal::make_nvp("LinearVelocity", linearVelocity));
		ar(cereal::make_nvp("Mass", mass));
		ar(cereal::make_nvp("DiagonalInertiaTensor", diagonalInertiaTensor));
		ar(cereal::make_nvp("MaxAngularVelocity", maxAngularVelocity));
		ar(cereal::make_nvp("MaxDepenetrationVelocity", maxDepenetrationVelocity));
		ar(cereal::make_nvp("IsAffectedByGravity", isAffectedByGravity));
		ar(cereal::make_nvp("IsKinematic", isKinematic));
		ar(cereal::make_nvp("CollisionDetectionMode", collisionDetectionMode));
		setAngularDrag(angularDrag);
		setAngularVelocity(angularVelocity);
		setCenterOfMass(centerOfMass);
		setLinearDrag(linearDrag);
		setLinearVelocity(linearVelocity);
		setMass(mass);
		setDiagonalInertiaTensor(diagonalInertiaTensor);
		setMaxAngularVelocity(maxAngularVelocity);
		setMaxDepenetrationVelocity(maxDepenetrationVelocity);
		affectByGravity(isAffectedByGravity);
		setAsKinematic(isKinematic);
		setCollisionDetectionMode(static_cast<Physics::CollisionDetectionMode>(collisionDetectionMode));
	}
}