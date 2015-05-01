#include "NullWorld.hpp"
#include "NullBoxCollider.hpp"
#include "NullCapsuleCollider.hpp"
#include "NullMeshCollider.hpp"
#include "NullSphereCollider.hpp"
#include "NullRaycaster.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullWorld::NullWorld(NullPhysics *physics)
			: WorldInterface(physics), gravity(GetDefaultGravity())
		{
			return;
		}

		// Inherited Methods
		void NullWorld::setGravity(const glm::vec3 &gravity)
		{
			this->gravity = gravity;
		}

		glm::vec3 NullWorld::getGravity(void) const
		{
			return gravity;
		}

		void NullWorld::enableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2)
		{
			return;
		}

		void NullWorld::disableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2)
		{
			return;
		}

		void NullWorld::simulate(float stepSize)
		{
			return;
		}

		RaycasterInterface *NullWorld::createRaycaster(void)
		{
			return new NullRaycaster(this);
		}

		RigidBodyInterface *NullWorld::createRigidBody(Private::GenericData *data)
		{
			return create<NullRigidBody>(this, data);
		}

		void NullWorld::destroyRigidBody(RigidBodyInterface *rigidBody)
		{
			destroy(static_cast<NullRigidBody *>(rigidBody));
		}

		ColliderInterface *NullWorld::createCollider(ColliderType colliderType, std::shared_ptr<MeshInstance> mesh, Private::GenericData *data)
		{
			switch (colliderType)
			{
				case ColliderType::Box:
					return create<NullBoxCollider>(this, data);
				case ColliderType::Capsule:
					return create<NullCapsuleCollider>(this, data);
				case ColliderType::Mesh:
					return create<NullMeshCollider>(this, mesh, data);
				case ColliderType::Sphere:
					return create<NullSphereCollider>(this, data);
				default:
					assert(!"Never reached");
					return nullptr;
			}
		}

		void NullWorld::destroyCollider(ColliderInterface *collider)
		{
			switch (collider->getColliderType())
			{
				case ColliderType::Box:
					destroy(dynamic_cast<NullBoxCollider *>(collider));
					break;
				case ColliderType::Capsule:
					destroy(dynamic_cast<NullCapsuleCollider *>(collider));
					break;
				case ColliderType::Mesh:
					destroy(dynamic_cast<NullMeshCollider *>(collider));
					break;
				case ColliderType::Sphere:
					destroy(dynamic_cast<NullSphereCollider *>(collider));
					break;
				default:
					assert(!"Never reached");
					break;
			}
		}

		MaterialInterface *NullWorld::createMaterial(const std::string &name)
		{
			MaterialTable::iterator found = materials.find(name);
			if (found != materials.end())
			{
				++found->second.second;
				return found->second.first;
			}
			else
			{
				return materials.insert(std::make_pair(name, std::make_pair(create<NullMaterial>(name), 1))).first->second.first;
			}
		}

		void NullWorld::destroyMaterial(MaterialInterface *material)
		{
			assert(material != nullptr && "Invalid material");
			MaterialTable::iterator found = materials.find(material->getName());
			if (found != materials.end())
			{
				--found->second.second;
				if (found->second.second == 0)
				{
					destroy(static_cast<NullMaterial *>(material));
					materials.erase(found);
				}
			}
		}
	}
}