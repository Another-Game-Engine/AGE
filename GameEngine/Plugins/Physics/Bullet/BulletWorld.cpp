#include <cassert>

#include <Serialize/BulletWorldImporter/btBulletWorldImporter.h>

#include "BulletWorld.hpp"
#include "BulletRigidBody.hpp"
#include "BulletMaterial.hpp"
#include "BulletBoxCollider.hpp"
#include "BulletCapsuleCollider.hpp"
#include "BulletMeshCollider.hpp"
#include "BulletSphereCollider.hpp"
#include "BulletRaycaster.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletWorld::BulletWorld(BulletPhysics *physics)
			: WorldInterface(physics)
		{
			for (std::size_t index = 0; index < sizeof(groupCollisionFlags) / sizeof(*groupCollisionFlags); ++index)
			{
				groupCollisionFlags[index] = 0xFFFFFFFF;
			}
			world.getPairCache()->setOverlapFilterCallback(this);
			world.getDispatchInfo().m_useContinuous = true;
			setGravity(GetDefaultGravity());
		}

		// Destructor
		BulletWorld::~BulletWorld(void)
		{
			collisionShapes.clear();
			world.getPairCache()->setOverlapFilterCallback(nullptr);
		}

		// Methods
		btCollisionShape *BulletWorld::getCollisionShape(const std::string &mesh, bool isConvex)
		{
			const std::string path = getAssetManager()->getAssetsDirectory() + mesh + "_bullet" + (isConvex ? "_convex" : "_concave") + ".phage";
			auto found = collisionShapes.find(path);
			if (found != collisionShapes.end())
			{
				return found->second.get();
			}
			OldFile filePath(path);
			if (!filePath.exists())
			{
				std::cerr << "Bullet file not found." << std::endl;
				return nullptr;
			}
			btBulletWorldImporter importer(getWorld());
			if (!importer.loadFile(path.c_str()))
			{
				std::cerr << "Bullet importer cannot open file." << std::endl;
				return nullptr;
			}
			if (importer.getNumCollisionShapes() <= 0)
			{
				std::cerr << "Bullet file is invalid. No collision shape inside." << std::endl;
				return nullptr;
			}
			std::shared_ptr<btCollisionShape> shape(importer.getCollisionShapeByIndex(0));
			collisionShapes.emplace(std::make_pair(path, shape));
			return shape.get();
		}

		btDiscreteDynamicsWorld *BulletWorld::getWorld(void)
		{
			return &world;
		}

		const btDiscreteDynamicsWorld *BulletWorld::getWorld(void) const
		{
			return &world;
		}

		void BulletWorld::updateCollisions(void)
		{
			CollisionListener *listener = getCollisionListener();
			std::vector<Contact> contacts;
			for (std::pair<Collider * const, std::unordered_map<Collider *, std::size_t>> &collisionPairs : collisions)
			{
				std::unordered_map<Collider *, std::size_t>::iterator it = collisionPairs.second.begin();
				std::unordered_map<Collider *, std::size_t>::iterator end = collisionPairs.second.end();
				while (it != end)
				{
					--it->second;
					if (it->second == 0)
					{
						listener->onCollision(collisionPairs.first, it->first, contacts, CollisionType::Lost);
						it = collisionPairs.second.erase(it);
					}
					else
					{
						++it;
					}
				}
			}
		}

		void BulletWorld::updateTriggers(void)
		{
			TriggerListener *listener = getTriggerListener();
			for (std::pair<Collider * const, std::unordered_map<Collider *, std::size_t>> &triggerPairs : triggers)
			{
				std::unordered_map<Collider *, std::size_t>::iterator it = triggerPairs.second.begin();
				std::unordered_map<Collider *, std::size_t>::iterator end = triggerPairs.second.end();
				while (it != end)
				{
					--it->second;
					if (it->second == 0)
					{
						listener->onTrigger(triggerPairs.first, it->first, TriggerType::Lost);
						it = triggerPairs.second.erase(it);
					}
					else
					{
						++it;
					}
				}
			}
		}

		void BulletWorld::processCollisionsAndTriggers(void)
		{
			const btDispatcher *dispatcher = world.getDispatcher();
			const int numberOfManifolds = dispatcher->getNumManifolds();
			for (int index = 0; index < numberOfManifolds; ++index)
			{
				const btPersistentManifold *contactManifold = const_cast<btDispatcher *>(dispatcher)->getManifoldByIndexInternal(index);
				const btRigidBody *body0 = btRigidBody::upcast(contactManifold->getBody0());
				const btRigidBody *body1 = btRigidBody::upcast(contactManifold->getBody1());
				const bool body0IsTrigger = (body0->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE) != 0;
				const bool body1IsTrigger = (body1->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE) != 0;
				if (body0 == nullptr || body1 == nullptr || (body0IsTrigger && body1IsTrigger))
				{
					continue;
				}
				const int numberOfContacts = contactManifold->getNumContacts();
				Collider *firstCollider = static_cast<ColliderInterface *>(static_cast<BulletCollider *>(body0->getUserPointer()))->getCollider();
				Collider *secondCollider = static_cast<ColliderInterface *>(static_cast<BulletCollider *>(body1->getUserPointer()))->getCollider();
				if (numberOfContacts > 0)
				{
					if (body0IsTrigger)
					{
						processTriggers(firstCollider, secondCollider);
					}
					else if (body1IsTrigger)
					{
						processTriggers(secondCollider, firstCollider);
					}
					else
					{
						processCollisions(firstCollider, secondCollider, contactManifold, numberOfContacts);
					}
				}
			}
		}

		void BulletWorld::processCollisions(Collider *firstCollider, Collider *secondCollider, const btPersistentManifold *contactManifold, int numberOfContacts)
		{
			CollisionListener *collisionListener = getCollisionListener();
			std::vector<Contact> firstColliderContacts;
			std::vector<Contact> secondColliderContacts;
			for (int contactNumber = 0; contactNumber < numberOfContacts; ++contactNumber)
			{
				const btManifoldPoint &contactPoint = contactManifold->getContactPoint(contactNumber);
				if (contactPoint.getDistance() < 0.0f)
				{
					const btVector3 &positionOnBody0 = contactPoint.getPositionWorldOnA();
					const btVector3 &positionOnBody1 = contactPoint.getPositionWorldOnB();
					const btVector3 normalOnBody0 = -contactPoint.m_normalWorldOnB;
					const btVector3 &normalOnBody1 = contactPoint.m_normalWorldOnB;
					firstColliderContacts.push_back({ glm::vec3(positionOnBody0.x(), positionOnBody0.y(), positionOnBody0.z()), glm::vec3(normalOnBody0.x(), normalOnBody0.y(), normalOnBody0.z()) });
					secondColliderContacts.push_back({ glm::vec3(positionOnBody1.x(), positionOnBody1.y(), positionOnBody1.z()), glm::vec3(normalOnBody1.x(), normalOnBody1.y(), normalOnBody1.z()) });
				}
			}
			std::size_t &hint = collisions[firstCollider][secondCollider];
			const CollisionType collisionType = hint == 0 ? CollisionType::New : CollisionType::Persistent;
			hint = hint == 0 ? 2 : hint + 1;
			collisions[secondCollider][firstCollider] = hint;
			collisionListener->onCollision(firstCollider, secondCollider, firstColliderContacts, collisionType);
			collisionListener->onCollision(secondCollider, firstCollider, secondColliderContacts, collisionType);
		}

		void BulletWorld::processTriggers(Collider *triggerCollider, Collider *otherCollider)
		{
			TriggerListener *triggerListener = getTriggerListener();
			std::size_t &hint = triggers[triggerCollider][otherCollider];
			const TriggerType triggerType = hint == 0 ? TriggerType::New : TriggerType::Persistent;
			hint = hint == 0 ? 2 : hint + 1;
			triggerListener->onTrigger(triggerCollider, otherCollider, triggerType);
		}

		// Inherited Methods
		void BulletWorld::setGravity(const glm::vec3 &gravity)
		{
			world.setGravity(btVector3(gravity.x, gravity.y, gravity.z));
		}

		glm::vec3 BulletWorld::getGravity(void) const
		{
			const btVector3 gravity = world.getGravity();
			return glm::vec3(gravity.x(), gravity.y(), gravity.z());
		}

		void BulletWorld::enableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2)
		{
			groupCollisionFlags[static_cast<std::uint8_t>(group1)] |= (1 << static_cast<std::uint8_t>(group2));
			groupCollisionFlags[static_cast<std::uint8_t>(group2)] |= (1 << static_cast<std::uint8_t>(group1));
		}

		void BulletWorld::disableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2)
		{
			groupCollisionFlags[static_cast<std::uint8_t>(group1)] &= ~(1 << static_cast<std::uint8_t>(group2));
			groupCollisionFlags[static_cast<std::uint8_t>(group2)] &= ~(1 << static_cast<std::uint8_t>(group1));
		}

		void BulletWorld::simulate(float stepSize)
		{
			world.stepSimulation(stepSize, 1, 1.0f / static_cast<float>(getTargetFPS()));
			processCollisionsAndTriggers();
			updateCollisions();
			updateTriggers();
		}

		RaycasterInterface *BulletWorld::createRaycaster(void)
		{
			return new BulletRaycaster(this);
		}

		RigidBodyInterface *BulletWorld::createRigidBody(Private::GenericData *data)
		{
			return create<BulletRigidBody>(this, data);
		}

		void BulletWorld::destroyRigidBody(RigidBodyInterface *rigidBody)
		{
			destroy(static_cast<BulletRigidBody *>(rigidBody));
		}

		ColliderInterface *BulletWorld::createCollider(ColliderType colliderType, const std::string &mesh, Private::GenericData *data)
		{
			switch (colliderType)
			{
				case ColliderType::Box:
					return create<BulletBoxCollider>(this, data);
				case ColliderType::Capsule:
					return create<BulletCapsuleCollider>(this, data);
				case ColliderType::Mesh:
					return create<BulletMeshCollider>(this, mesh, data);
				case ColliderType::Sphere:
					return create<BulletSphereCollider>(this, data);
				default:
					assert(!"Never reached");
					return nullptr;
			}
		}

		void BulletWorld::destroyCollider(ColliderInterface *collider)
		{
			Collider *colliderComponent = collider->getCollider();
			for (std::pair<Collider * const, std::size_t> &pair : collisions[colliderComponent])
			{
				collisions[pair.first].erase(colliderComponent);
			}
			collisions.erase(colliderComponent);
			for (std::pair<Collider * const, std::unordered_map<Collider *, std::size_t>> &triggerPairs : triggers)
			{
				triggerPairs.second.erase(colliderComponent);
			}
			triggers.erase(colliderComponent);
			switch (collider->getColliderType())
			{
				case ColliderType::Box:
					destroy(dynamic_cast<BulletBoxCollider *>(collider));
					break;
				case ColliderType::Capsule:
					destroy(dynamic_cast<BulletCapsuleCollider *>(collider));
					break;
				case ColliderType::Mesh:
					destroy(dynamic_cast<BulletMeshCollider *>(collider));
					break;
				case ColliderType::Sphere:
					destroy(dynamic_cast<BulletSphereCollider *>(collider));
					break;
				default:
					assert(!"Never reached");
					break;
			}
		}

		MaterialInterface *BulletWorld::createMaterial(const std::string &name)
		{
			MaterialTable::iterator found = materials.find(name);
			if (found != materials.end())
			{
				++found->second.second;
				return found->second.first;
			}
			else
			{
				return materials.insert(std::make_pair(name, std::make_pair(create<BulletMaterial>(name), 1))).first->second.first;
			}
		}

		void BulletWorld::destroyMaterial(MaterialInterface *material)
		{
			assert(material != nullptr && "Invalid material");
			MaterialTable::iterator found = materials.find(material->getName());
			if (found != materials.end())
			{
				--found->second.second;
				if (found->second.second == 0)
				{
					destroy(static_cast<BulletMaterial *>(material));
					materials.erase(found);
				}
			}
		}

		bool BulletWorld::needBroadphaseCollision(btBroadphaseProxy *proxy1, btBroadphaseProxy *proxy2) const
		{
			const std::uint32_t shapeGroup1 = static_cast<std::uint32_t>(proxy1->m_collisionFilterGroup) & 31;
			const std::uint32_t shapeGroup2 = static_cast<std::uint32_t>(proxy2->m_collisionFilterGroup) & 31;
			return (groupCollisionFlags[shapeGroup1] & (1 << shapeGroup2)) != 0;
		}
	}
}