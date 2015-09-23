#include <Physics/Trigger.hpp>

namespace AGE
{
	namespace Physics
	{
		// Constructors
		Trigger::Trigger(Collider *collider, TriggerType triggerType)
			: collider(collider), triggerType(triggerType)
		{
			return;
		}

		Trigger::Trigger(Trigger &&other)
			: collider(other.collider), triggerType(other.triggerType)
		{
			other.collider = nullptr;
		}

		// Assignment Operators
		Trigger &Trigger::operator=(Trigger &&other)
		{
			if (this != &other)
			{
				collider = other.collider;
				other.collider = nullptr;
				triggerType = other.triggerType;
			}
			return *this;
		}

		// Methods
		Collider *Trigger::getCollider(void)
		{
			return collider;
		}

		const Collider *Trigger::getCollider(void) const
		{
			return collider;
		}

		TriggerType Trigger::getTriggerType(void) const
		{
			return triggerType;
		}
	}
}