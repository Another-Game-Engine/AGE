#ifndef  __COLLISION_ADDER_SYSTEM_HPP__
# define __COLLISION_ADDER_SYSTEM_HPP__

#include <Systems/System.h>
#include <Entities/EntityData.hh>
#include <Core/Engine.hh>
#include <Components/Collision.hpp>
#include <Managers/BulletCollisionManager.hpp>


class CollisionAdder : public System
{
public:
	CollisionAdder(AScene *scene) : System(scene)
		, _manager(scene->getEngine().getInstance<BulletCollisionManager>())
	{}
	virtual ~CollisionAdder(){}
private:
	BulletCollisionManager &_manager;
	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		btDispatcher *dispatcher = _manager.getWorld()->getDispatcher();
		unsigned int max = dispatcher->getNumManifolds();
		for (unsigned int i = 0; i < max; ++i)
		{
			btPersistentManifold *contact = dispatcher->getManifoldByIndexInternal(i);
			const btCollisionObject *oa = static_cast<const btCollisionObject*>(contact->getBody0());
			const btCollisionObject *ob = static_cast<const btCollisionObject*>(contact->getBody1());
			float maxContact = 0;
			for (auto i = 0, mi = contact->getNumContacts(); i < mi; ++i)
				if (contact->getContactPoint(i).m_appliedImpulse > maxContact)
					maxContact = contact->getContactPoint(i).m_appliedImpulse;
			Entity h1 = *(static_cast<Entity*>(oa->getUserPointer()));
			EntityData *e1 = h1.get();
			auto c1 = e1->addComponent<Component::Collision>();

			Entity h2 = *(static_cast<Entity*>(ob->getUserPointer()));
			EntityData *e2 = h2.get();
			auto c2 = e2->addComponent<Component::Collision>();
			c1->addCollision(h2);
			c1->force = c1->force < maxContact ? maxContact : c1->force;
			c2->addCollision(h1); 
			c2->force = c2->force < maxContact ? maxContact : c2->force;
		}
	}

	virtual void initialize()
	{
	}
};

#endif   //__COLLISION_ADDER_SYSTEM_HPP__