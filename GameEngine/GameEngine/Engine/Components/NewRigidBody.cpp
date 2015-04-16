#include <Components/NewRigidBody.hpp>
#include <Components/Collider.hpp>
#include <Components/PhysicsData.hpp>
#include <Core/AScene.hh>
#include <Physics/WorldInterface.hpp>

namespace AGE
{
	// Methods
	void NewRigidBody::init(void)
	{
		assert(rigidBody == nullptr && "NewRigidBody already initialized");
		rigidBody = entity.getScene()->getInstance<Physics::WorldInterface>()->createRigidBody(entity.addComponent<Private::PhysicsData>()->getData());
		rigidBody->rigidBody = this;
	}

	void NewRigidBody::setAngularDrag(float angularDrag)
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		rigidBody->setAngularDrag(angularDrag);
	}

	float NewRigidBody::getAngularDrag(void) const
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		return rigidBody->getAngularDrag();
	}

	void NewRigidBody::setAngularVelocity(const glm::vec3 &angularVelocity)
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		rigidBody->setAngularVelocity(angularVelocity);
	}

	glm::vec3 NewRigidBody::getAngularVelocity(void) const
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		return rigidBody->getAngularVelocity();
	}

	void NewRigidBody::setCenterOfMass(const glm::vec3 &centerOfMass)
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		rigidBody->setCenterOfMass(centerOfMass);
	}

	glm::vec3 NewRigidBody::getCenterOfMass(void) const
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		return rigidBody->getCenterOfMass();
	}

	void NewRigidBody::setLinearDrag(float linearDrag)
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		rigidBody->setLinearDrag(linearDrag);
	}

	float NewRigidBody::getLinearDrag(void) const
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		return rigidBody->getLinearDrag();
	}

	void NewRigidBody::setLinearVelocity(const glm::vec3 &linearVelocity)
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		rigidBody->setLinearVelocity(linearVelocity);
	}

	glm::vec3 NewRigidBody::getLinearVelocity(void) const
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		return rigidBody->getLinearVelocity();
	}

	void NewRigidBody::setMass(float mass)
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		rigidBody->setMass(mass);
	}

	float NewRigidBody::getMass(void) const
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		return rigidBody->getMass();
	}

	void NewRigidBody::setDiagonalInertiaTensor(const glm::vec3 &diagonalInertiaTensor)
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		rigidBody->setDiagonalInertiaTensor(diagonalInertiaTensor);
	}

	glm::vec3 NewRigidBody::getDiagonalInertiaTensor(void) const
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		return rigidBody->getDiagonalInertiaTensor();
	}

	void NewRigidBody::setMaxAngularVelocity(float maxAngularVelocity)
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		rigidBody->setMaxAngularVelocity(maxAngularVelocity);
	}

	float NewRigidBody::getMaxAngularVelocity(void) const
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		return rigidBody->getMaxAngularVelocity();
	}

	void NewRigidBody::setMaxDepenetrationVelocity(float maxDepenetrationVelocity)
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		rigidBody->setMaxDepenetrationVelocity(maxDepenetrationVelocity);
	}

	float NewRigidBody::getMaxDepenetrationVelocity(void) const
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		return rigidBody->getMaxDepenetrationVelocity();
	}

	void NewRigidBody::setPosition(const glm::vec3 &position)
	{
		static bool FirstCall = true;
		if (FirstCall)
		{
			FirstCall = false;
			assert(rigidBody != nullptr && "Invalid NewRigidBody");
			rigidBody->setPosition(position);
			entity.getLink().setPosition(position);
			FirstCall = true;
		}
	}

	glm::vec3 NewRigidBody::getPosition(void) const
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		return rigidBody->getPosition();
	}

	void NewRigidBody::setRotation(const glm::quat &rotation)
	{
		static bool FirstCall = true;
		if (FirstCall)
		{
			FirstCall = false;
			assert(rigidBody != nullptr && "Invalid NewRigidBody");
			rigidBody->setRotation(rotation);
			entity.getLink().setOrientation(rotation);
			FirstCall = true;
		}
	}

	glm::quat NewRigidBody::getRotation(void) const
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		return rigidBody->getRotation();
	}

	void NewRigidBody::affectByGravity(bool mustBeAffectedByGravity)
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		rigidBody->affectByGravity(mustBeAffectedByGravity);
	}

	bool NewRigidBody::isAffectedByGravity(void) const
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		return rigidBody->isAffectedByGravity();
	}

	void NewRigidBody::setAsKinematic(bool mustBeKinematic)
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		rigidBody->setAsKinematic(mustBeKinematic);
	}

	bool NewRigidBody::isKinematic(void) const
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		return rigidBody->isKinematic();
	}

	void NewRigidBody::setCollisionDetectionMode(Physics::CollisionDetectionMode collisionDetectionMode)
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		rigidBody->setCollisionDetectionMode(collisionDetectionMode);
	}

	Physics::CollisionDetectionMode NewRigidBody::getCollisionDetectionMode(void) const
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		return rigidBody->getCollisionDetectionMode();
	}

	void NewRigidBody::addExplosionForce(float explosionForce, const glm::vec3 &explosionPosition, float explosionRadius, Physics::ForceMode forceMode)
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		rigidBody->addExplosionForce(explosionForce, explosionPosition, explosionRadius, forceMode);
	}

	void NewRigidBody::addForce(const glm::vec3 &force, Physics::ForceMode forceMode)
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		rigidBody->addForce(force, forceMode);
	}

	void NewRigidBody::addForceAtWorldPosition(const glm::vec3 &force, const glm::vec3 &position, Physics::ForceMode forceMode)
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		rigidBody->addForceAtWorldPosition(force, position, forceMode);
	}

	void NewRigidBody::addForceAtLocalPosition(const glm::vec3 &force, const glm::vec3 &position, Physics::ForceMode forceMode)
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		rigidBody->addForceAtLocalPosition(force, position, forceMode);
	}

	void NewRigidBody::addAbsoluteTorque(const glm::vec3 &torque, Physics::ForceMode forceMode)
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		rigidBody->addAbsoluteTorque(torque, forceMode);
	}

	void NewRigidBody::addRelativeTorque(const glm::vec3 &torque, Physics::ForceMode forceMode)
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		rigidBody->addRelativeTorque(torque, forceMode);
	}

	glm::vec3 NewRigidBody::getVelocityAtWorldPosition(const glm::vec3 &position) const
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		return rigidBody->getVelocityAtWorldPosition(position);
	}

	glm::vec3 NewRigidBody::getVelocityAtLocalPosition(const glm::vec3 &position) const
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		return rigidBody->getVelocityAtLocalPosition(position);
	}

	// Inherited Methods
	void NewRigidBody::reset(void)
	{
		if (rigidBody != nullptr)
		{
			entity.getScene()->getInstance<Physics::WorldInterface>()->destroyRigidBody(rigidBody);
			rigidBody = nullptr;
		}
		if (!entity.haveComponent<Collider>())
		{
			entity.removeComponent<Private::PhysicsData>();
		}
	}
}