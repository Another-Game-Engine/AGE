#pragma once

#include <Physics/TriggerType.hpp>

namespace AGE
{
	class Collider;

	namespace Physics
	{
		class Trigger final
		{
		public:
			// Constructors
			Trigger(void) = delete;

			Trigger(Collider *collider, TriggerType triggerType);

			Trigger(const Trigger &) = default;

			Trigger(Trigger &&other);

			// Assignment Operators
			Trigger &operator=(const Trigger &) = default;

			Trigger &operator=(Trigger &&);

			// Destructor
			~Trigger(void) = default;

			// Methods
			Collider *getCollider(void);

			const Collider *getCollider(void) const;

			TriggerType getTriggerType(void) const;

		private:
			// Attributes
			Collider *collider = nullptr;

			TriggerType triggerType;
		};
	}
}
