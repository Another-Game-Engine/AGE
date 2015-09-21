#pragma once

#include "TriggerType.hpp"

namespace AGE
{
	class Collider;

	namespace Physics
	{
		class TriggerListener
		{
		public:
			// Constructors
			TriggerListener(void) = default;

			TriggerListener(const TriggerListener &) = default;

			// Assignment Operators
			TriggerListener &operator=(const TriggerListener &) = default;

			// Destructors
			virtual ~TriggerListener(void) = default;

			// Virtual Methods
			virtual void onTrigger(Collider *triggerCollider, Collider *otherCollider, TriggerType triggerType) = 0;
		};
	}
}