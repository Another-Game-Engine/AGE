#include <Components/Collider.hpp>
#include <Components/NewRigidBody.hpp>
#include <Components/PhysicsData.hpp>
#include <Core/AScene.hh>
#include <Physics/WorldInterface.hpp>

namespace AGE
{
	// Methods
	void Collider::init(Physics::ColliderType colliderType)
	{
		assert(collider == nullptr && "Collider already initialized");
		collider = entity.getScene()->getInstance<Physics::WorldInterface>()->createCollider(colliderType, entity.addComponent<Private::PhysicsData>()->getData());
	}

	void Collider::setStaticFriction(float staticFriction)
	{
		assert(collider != nullptr && "Invalid Collider");
		collider->getMaterial()->setStaticFriction(staticFriction);
	}

	float Collider::getStaticFriction(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		return collider->getMaterial()->getStaticFriction();
	}

	void Collider::setDynamicFriction(float dynamicFriction)
	{
		assert(collider != nullptr && "Invalid Collider");
		collider->getMaterial()->setDynamicFriction(dynamicFriction);
	}

	float Collider::getDynamicFriction(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		return collider->getMaterial()->getDynamicFriction();
	}

	void Collider::setRestitution(float restitution)
	{
		assert(collider != nullptr && "Invalid Collider");
		collider->getMaterial()->setRestitution(restitution);
	}

	float Collider::getRestitution(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		return collider->getMaterial()->getRestitution();
	}

	Physics::ColliderType Collider::getColliderType(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		return collider->getColliderType();
	}

	void Collider::setAsTrigger(bool mustBeATrigger)
	{
		assert(collider != nullptr && "Invalid Collider");
		collider->setAsTrigger(mustBeATrigger);
	}

	bool Collider::isATrigger(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		return collider->isATrigger();
	}

	void Collider::setFilterGroup(const std::string &filterName)
	{
		assert(collider != nullptr && "Invalid Collider");
		collider->setFilterGroup(collider->getWorld()->getFilterGroupForFilterName(filterName));
	}

	void Collider::setFilterGroup(Physics::FilterGroup group)
	{
		assert(collider != nullptr && "Invalid Collider");
		collider->setFilterGroup(group);
	}

	const std::string &Collider::getFilterGroupName(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		return collider->getWorld()->getFilterNameForFilterGroup(collider->getFilterGroup());
	}

	Physics::FilterGroup Collider::getFilterGroup(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		return collider->getFilterGroup();
	}

	void Collider::setCenter(const glm::vec3 &center)
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Box:
				collider->as<Physics::ColliderType::Box>()->setCenter(center);
				break;
			case Physics::ColliderType::Capsule:
				collider->as<Physics::ColliderType::Capsule>()->setCenter(center);
				break;
			case Physics::ColliderType::Sphere:
				collider->as<Physics::ColliderType::Sphere>()->setCenter(center);
				break;
			default:
				assert(!"Invalid collider type");
				break;
		}
	}

	glm::vec3 Collider::getCenter(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Box:
				return collider->as<Physics::ColliderType::Box>()->getCenter();
			case Physics::ColliderType::Capsule:
				return collider->as<Physics::ColliderType::Capsule>()->getCenter();
			case Physics::ColliderType::Sphere:
				return collider->as<Physics::ColliderType::Sphere>()->getCenter();
			default:
				assert(!"Invalid collider type");
				return glm::vec3();
		}

	}

	void Collider::setHeight(float height)
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Capsule:
				collider->as<Physics::ColliderType::Capsule>()->setHeight(height);
				break;
			default:
				assert(!"Invalid collider type");
				break;
		}
	}

	float Collider::getHeight(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Capsule:
				return collider->as<Physics::ColliderType::Capsule>()->getHeight();
			default:
				assert(!"Invalid collider type");
				return 0.0f;
		}
	}

	void Collider::setRadius(float radius)
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Capsule:
				collider->as<Physics::ColliderType::Capsule>()->setRadius(radius);
				break;
			case Physics::ColliderType::Sphere:
				collider->as<Physics::ColliderType::Sphere>()->setRadius(radius);
				break;
			default:
				assert(!"Invalid collider type");
				break;
		}
	}

	float Collider::getRadius(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Capsule:
				return collider->as<Physics::ColliderType::Capsule>()->getRadius();
			case Physics::ColliderType::Sphere:
				return collider->as<Physics::ColliderType::Sphere>()->getRadius();
			default:
				assert(!"Invalid collider type");
				return 0.0f;
		}
	}

	void Collider::setSize(const glm::vec3 &size)
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Box:
				collider->as<Physics::ColliderType::Box>()->setSize(size);
				break;
			default:
				assert(!"Invalid collider type");
				break;
		}
	}

	glm::vec3 Collider::getSize(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Box:
				return collider->as<Physics::ColliderType::Box>()->getSize();
			default:
				assert(!"Invalid collider type");
				return glm::vec3();
		}
	}

	// Inherited Methods
	void Collider::reset(void)
	{
		if (collider != nullptr)
		{
			entity.getScene()->getInstance<Physics::WorldInterface>()->destroyCollider(collider);
			collider = nullptr;
		}
		if (!entity.haveComponent<NewRigidBody>())
		{
			entity.removeComponent<Private::PhysicsData>();
		}
	}
}