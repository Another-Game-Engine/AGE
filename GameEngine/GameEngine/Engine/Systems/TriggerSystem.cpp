#include <Systems/TriggerSystem.hpp>
#include <Physics/Trigger.hpp>

namespace AGE
{
	namespace Private
	{
		TriggerSystem::TriggerSystem(AScene *scene)
			: System(scene), entityFilter(scene)
		{
			_name = "TriggerSystem";
			entityFilter.requireComponent<Collider>();
		}

		bool TriggerSystem::initialize(void)
		{
			_scene->getInstance<Physics::WorldInterface>()->setTriggerListener(this);
			return true;
		}

		void TriggerSystem::finalize(void)
		{
			_scene->getInstance<Physics::WorldInterface>()->setTriggerListener(nullptr);
		}

		void TriggerSystem::mainUpdate(float elapsedTime)
		{
			for (Entity entity : entityFilter.getCollection())
			{
				entity.getComponent<Collider>()->triggers.clear();
			}
		}

		void TriggerSystem::onTrigger(Collider *triggerCollider, Collider *otherCollider, Physics::TriggerType triggerType)
		{
			triggerCollider->triggers.push_back(Physics::Trigger(otherCollider, triggerType));
		}
	}
}