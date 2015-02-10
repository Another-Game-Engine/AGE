#pragma once

#include <Physic/Utils/BtConversion.hpp>
#include <Systems/System.h>
#include <Components/RigidBody.hpp>
#include <Physic/BulletDynamicManager.hpp>
#include <Core/Engine.hh>
#include <Entities/EntityFlags.hh>

#include <future>

namespace AGE
{
	class BulletDynamicSystem : public System
	{
	public:
		BulletDynamicSystem(std::weak_ptr<AScene> &&scene)
			: System(std::move(scene))
			, _manager(nullptr)
			, _filter(std::move(scene))
		{
			_name = "bullet_dynamic_system";
			_manager = dynamic_cast<BulletDynamicManager*>(_scene.lock()->getInstance<BulletCollisionManager>());
			_filter.requireComponent<RigidBody>();
		}
		virtual ~BulletDynamicSystem(){}
	private:
		BulletDynamicManager* _manager;
		EntityFilter _filter;

		virtual void updateBegin(double time)
		{
			auto scene = _scene.lock();
			for (auto e : _filter.getCollection())
			{
				if (scene->getLink(e)->isUserModified())
				{
					auto rb = scene->getComponent<RigidBody>(e);
					if (rb->getMass() == 0)
					{
						rb->setTransformation(scene->getLink(e));
						scene->getLink(e)->setUserModified(false);
					}
				}

			}
			_manager->getWorld()->stepSimulation(static_cast<btScalar>(time), 10);
		}

		virtual void updateEnd(double time)
		{}

		virtual void mainUpdate(double time)
		{
		}

		virtual bool initialize()
		{
			return true;
		}
	};
}