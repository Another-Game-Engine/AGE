#pragma once

#include <Systems/System.h>
#include <Core/Engine.hh>
#include <Components/Collision.hpp>
#include <Physic/BulletCollisionManager.hpp>

namespace AGE
{
	class CollisionAdder : public System
	{
	public:
		CollisionAdder(std::weak_ptr<AScene> &&scene)
			: System(std::move(scene))
			, _manager(scene.lock()->getInstance<BulletCollisionManager>())
		{
			_name = "collision_adder_system";
		}
		virtual ~CollisionAdder(){}
	private:
		std::shared_ptr<BulletCollisionManager> _manager;
		virtual void updateBegin(double time)
		{
		}

		virtual void updateEnd(double time)
		{}

		virtual void mainUpdate(double time)
		{
			SpuGatheringCollisionDispatcher *dispatcher = static_cast<SpuGatheringCollisionDispatcher*>(_manager->getWorld()->getDispatcher());
			auto scene = _scene.lock();
			unsigned int max = dispatcher->getNumManifolds();
			for (unsigned int i = 0; i < max; ++i)
			{
				btPersistentManifold *contact = dispatcher->getManifoldByIndexInternal(i);
				const btCollisionObject *oa = static_cast<const btCollisionObject*>(contact->getBody0());
				const btCollisionObject *ob = static_cast<const btCollisionObject*>(contact->getBody1());
				float maxContact = 0;
				for (auto j = 0, mj = contact->getNumContacts(); j < mj; ++j)
					if (contact->getContactPoint(j).m_appliedImpulse > maxContact)
						maxContact = contact->getContactPoint(j).m_appliedImpulse;
				Entity h1 = *(static_cast<Entity*>(oa->getUserPointer()));
				Entity h2 = *(static_cast<Entity*>(ob->getUserPointer()));

				auto c2 = scene->addComponent<Component::Collision>(h2);
				c2->addCollision(h1);
				c2->force = c2->force < maxContact ? maxContact : c2->force;

				auto c1 = scene->addComponent<Component::Collision>(h1);
				c1->addCollision(h2);
				c1->force = c1->force < maxContact ? maxContact : c1->force;
			}
		}

		virtual bool initialize()
		{
			return true;
		}
	};
}