#include <Components/Collider.hpp>
#include <Components/RigidBody.hpp>
#include <Components/PhysicsData.hpp>
#include <Core/AScene.hh>
#include <Physics/PhysicsInterface.hpp>
#include <Physics/WorldInterface.hpp>
#include <AssetManagement/AssetManager.hh>

namespace AGE
{
	// Methods
	void Collider::init(Physics::ColliderType colliderType, const std::string &mesh)
	{
		assert(collider == nullptr && "Collider already initialized");
		collider = entity.getScene()->getInstance<Physics::PhysicsInterface>()->getWorld()->createCollider(colliderType, colliderType == Physics::ColliderType::Mesh ? entity.getScene()->getInstance<AGE::AssetsManager>()->getMesh(mesh) : nullptr, entity.addComponent<Private::PhysicsData>()->getData());
		collider->collider = this;
		scale(entity.getLink().getScale());
	}

	void Collider::setMaterial(const std::string &material)
	{
		assert(collider != nullptr && "Invalid Collider");
		collider->setMaterial(material);
	}

	Physics::MaterialInterface *Collider::getMaterial(void)
	{
		assert(collider != nullptr && "Invalid Collider");
		return collider->getMaterial();
	}

	const Physics::MaterialInterface *Collider::getMaterial(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		return collider->getMaterial();
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

	void Collider::setMesh(const std::string &mesh)
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Mesh:
				collider->as<Physics::ColliderType::Mesh>()->setMesh(entity.getScene()->getInstance<AGE::AssetsManager>()->getMesh(mesh));
				break;
			default:
				assert(!"Invalid collider type");
				break;
		}
	}

	std::shared_ptr<MeshInstance> Collider::getMesh(void)
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Mesh:
				return collider->as<Physics::ColliderType::Mesh>()->getMesh();
			default:
				assert(!"Invalid collider type");
				return nullptr;
		}
	}

	std::shared_ptr<const MeshInstance> Collider::getMesh(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Mesh:
				return collider->as<Physics::ColliderType::Mesh>()->getMesh();
			default:
				assert(!"Invalid collider type");
				return nullptr;
		}
	}

	void Collider::setAsConvex(bool mustBeConvex)
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Mesh:
				collider->as<Physics::ColliderType::Mesh>()->setAsConvex(mustBeConvex);
				break;
			default:
				assert(!"Invalid collider type");
				break;
		}
	}

	bool Collider::isConvex(void) const
	{
		assert(collider != nullptr && "Invalid Collider");
		switch (getColliderType())
		{
			case Physics::ColliderType::Mesh:
				return collider->as<Physics::ColliderType::Mesh>()->isConvex();
			default:
				assert(!"Invalid collider type");
				return true;
		}
	}

	void Collider::scale(const glm::vec3 &scaling)
	{
		assert(collider != nullptr && "Invalid Collider");
		collider->scale(scaling);
	}

	// Inherited Methods
	void Collider::reset(void)
	{
		if (collider != nullptr)
		{
			entity.getScene()->getInstance<Physics::PhysicsInterface>()->getWorld()->destroyCollider(collider);
			collider = nullptr;
		}
		if (!entity.haveComponent<RigidBody>())
		{
			entity.removeComponent<Private::PhysicsData>();
		}
	}
}