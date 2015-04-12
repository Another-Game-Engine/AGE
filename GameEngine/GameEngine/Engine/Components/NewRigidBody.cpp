#include <Components/NewRigidBody.hpp>
#include <Core/AScene.hh>
#include <Physics/WorldInterface.hpp>

namespace AGE
{
	// Methods
	void NewRigidBody::init(void)
	{
		assert(rigidBody == nullptr && "NewRigidBody already initialized");
		rigidBody = entity.getScene()->getInstance<Physics::WorldInterface>()->createRigidBody(entity.getLink().getPosition());
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
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		rigidBody->setPosition(position);
		entity.getLink().setPosition(position);
	}

	glm::vec3 NewRigidBody::getPosition(void) const
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		return rigidBody->getPosition();
	}

	void NewRigidBody::setRotation(const glm::quat &rotation)
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		rigidBody->setRotation(rotation);
		entity.getLink().setOrientation(rotation);
	}

	glm::quat NewRigidBody::getRotation(void) const
	{
		assert(rigidBody != nullptr && "Invalid NewRigidBody");
		return rigidBody->getRotation();
	}

	// Inherited Methods
	void NewRigidBody::reset(void)
	{
		if (rigidBody != nullptr)
		{
			entity.getScene()->getInstance<Physics::WorldInterface>()->destroyRigidBody(rigidBody);
			rigidBody = nullptr;
		}
	}
}