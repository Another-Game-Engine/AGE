#include <Systems/TriggerSystem.hpp>
#include <Physics/Trigger.hpp>
#include <Physics/PhysicsInterface.hpp>

namespace AGE
{
	namespace Private
	{
		// Constructors
		TriggerSystem::TriggerSystem(AScene *scene)
			: System(scene), entityFilter(scene)
		{
			_name = "TriggerSystem";
			entityFilter.requireComponent<Collider>();
		}

		// Methods
		void TriggerSystem::addListener(Physics::ITriggerListener *listener)
		{
			listeners.insert(listener);
		}

		void TriggerSystem::removeListener(Physics::ITriggerListener *listener)
		{
			listeners.erase(listener);
		}

		// Inherited Methods
		bool TriggerSystem::initialize(void)
		{
			_scene->getInstance<Physics::PhysicsInterface>()->getWorld()->setTriggerListener(this);
			return true;
		}

		void TriggerSystem::finalize(void)
		{
			_scene->getInstance<Physics::PhysicsInterface>()->getWorld()->setTriggerListener(nullptr);
		}

		void TriggerSystem::mainUpdate(float elapsedTime)
		{
			for (const Entity &entity : entityFilter.getCollection())
			{
				entity->getComponent<Collider>()->triggers.clear();
			}
		}

		void TriggerSystem::onTrigger(Collider *triggerCollider, Collider *otherCollider, Physics::TriggerType triggerType)
		{
			triggerCollider->triggers.push_back(Physics::Trigger(otherCollider, triggerType));
			switch (triggerType)
			{
				case Physics::TriggerType::New:
				{
					for (Physics::ITriggerListener *listener : listeners)
					{
						listener->onTriggerEnter(triggerCollider->entity, otherCollider->entity);
					}
					break;
				}
				case Physics::TriggerType::Persistent:
				{
					for (Physics::ITriggerListener *listener : listeners)
					{
						listener->onTriggerStay(triggerCollider->entity, otherCollider->entity);
					}
					break;
				}
				case Physics::TriggerType::Lost:
				{
					for (Physics::ITriggerListener *listener : listeners)
					{
						listener->onTriggerExit(triggerCollider->entity, otherCollider->entity);
					}
					break;
				}
				default:
				{
					assert(!"Never reached");
					break;
				}
			}
		}
	}
}