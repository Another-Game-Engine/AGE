#include "Systems/BulletDynamicSystem.hpp"
#include <Components/RigidBody.hpp>

namespace AGE
{
	BulletDynamicSystem::BulletDynamicSystem(AScene *scene)
		: System(scene)
		, _manager(dynamic_cast<BulletDynamicManager*>(_scene->getInstance<BulletCollisionManager>()))
		, _filter(scene)
	{
		_name = "bullet_dynamic_system";
		_filter.requireComponent<RigidBody>();
	}

	void BulletDynamicSystem::updateBegin(float time)
	{		
		auto scene = _scene;
		for (auto e : _filter.getCollection())
		{
			if (e.getLink().isUserModified())
			{
				auto rb = e.getComponent<RigidBody>();
				if (rb->getMass() == 0)
				{
					rb->setTransformation(&e.getLink());
					e.getLink().setUserModified(false);
				}
			}

		}
		if (time == 0.0f)
		{
			return;
		}
		_manager->getWorld()->stepSimulation(static_cast<btScalar>(time), 10);
		registerContactInformations();
	}

	void BulletDynamicSystem::registerContactInformations(void) const
	{
		btDispatcher *dispatcher = _manager->getWorld()->getDispatcher();
		const int numberOfManifolds = dispatcher->getNumManifolds();
		for (int manifoldIndex = 0; manifoldIndex < numberOfManifolds; ++manifoldIndex)
		{
			const btPersistentManifold *contactManifold = dispatcher->getManifoldByIndexInternal(manifoldIndex);
			Entity &entityA = *static_cast<Entity *>((static_cast<const btCollisionObject *>(contactManifold->getBody0()))->getUserPointer());
			Entity &entityB = *static_cast<Entity *>((static_cast<const btCollisionObject *>(contactManifold->getBody1()))->getUserPointer());
			RigidBody *rigidBodyEntityA = entityA.getComponent<RigidBody>();
			RigidBody *rigidBodyEntityB = entityB.getComponent<RigidBody>();
			const int numberOfContacts = contactManifold->getNumContacts();
			for (int contactIndex = 0; contactIndex < numberOfContacts; ++contactIndex)
			{
				const btManifoldPoint &contactPoint = contactManifold->getContactPoint(contactIndex);
				if (contactPoint.getDistance() < 0.0f)
				{
					const btVector3 &pointA = contactPoint.getPositionWorldOnA();
					const btVector3 &pointB = contactPoint.getPositionWorldOnB();
					const btVector3 &normalOnB = contactPoint.m_normalWorldOnB;
					const btVector3 normalOnA = -normalOnB;
					rigidBodyEntityA->_addContactInformation(entityB, pointA, normalOnA);
					rigidBodyEntityB->_addContactInformation(entityA, pointB, normalOnB);
				}
			}
		}
	}
}
