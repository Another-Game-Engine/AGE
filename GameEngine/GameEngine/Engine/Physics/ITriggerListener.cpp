#include <cassert>

#include <Physics/ITriggerListener.hpp>
#include <Systems/TriggerSystem.hpp>

namespace AGE
{
	namespace Physics
	{
		// Constructors
		ITriggerListener::ITriggerListener(AScene *scene)
			: scene(scene)
		{
			assert(scene != nullptr && "Invalid scene");
			std::shared_ptr<::AGE::Private::TriggerSystem> triggerSystem = scene->getSystem<::AGE::Private::TriggerSystem>();
			assert(triggerSystem != nullptr && "TriggerSystem not found");
			triggerSystem->addListener(this);
		}

		// Destructors
		ITriggerListener::~ITriggerListener(void)
		{
			std::shared_ptr<::AGE::Private::TriggerSystem> triggerSystem = scene->getSystem<::AGE::Private::TriggerSystem>();
			assert(triggerSystem != nullptr && "TriggerSystem not found");
			triggerSystem->removeListener(this);
		}

		// Virtual Methods
		void ITriggerListener::onTriggerEnter(Entity triggerEntity, Entity otherEntity)
		{
			return;
		}

		void ITriggerListener::onTriggerStay(Entity triggerEntity, Entity otherEntity)
		{
			return;
		}

		void ITriggerListener::onTriggerExit(Entity triggerEntity, Entity otherEntity)
		{
			return;
		}
	}
}