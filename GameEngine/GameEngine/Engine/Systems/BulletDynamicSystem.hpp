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
		BulletDynamicSystem(AScene *scene)
			: System(std::move(scene))
			, _manager(nullptr)
			, _filter(std::move(scene))
		{
			_name = "bullet_dynamic_system";
			_manager = dynamic_cast<BulletDynamicManager*>(_scene->getInstance<BulletCollisionManager>());
			_filter.requireComponent<RigidBody>();
		}
		virtual ~BulletDynamicSystem(){}
	private:
		BulletDynamicManager* _manager;
		EntityFilter _filter;

		virtual void updateBegin(double time)
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