#pragma once

#include <Systems/System.h>
#include <Physics/TriggerListener.hpp>
#include <Components/Collider.hpp>

namespace AGE
{
	namespace Private
	{
		class TriggerSystem final : public System, public Physics::TriggerListener
		{
		public:
			// Constructors
			TriggerSystem(void) = delete;

			TriggerSystem(AScene *scene);

			TriggerSystem(TriggerSystem const &) = delete;

			// Assignment Operators
			TriggerSystem &operator=(TriggerSystem const &) = delete;

			// Destructors
			~TriggerSystem(void) = default;

		private:
			// Attributes
			EntityFilter entityFilter;

			// Inherited Methods
			bool initialize(void) override final;

			void finalize(void) override final;

			void mainUpdate(float elapsedTime) override final;

			void onTrigger(Collider *triggerCollider, Collider *otherCollider, Physics::TriggerType triggerType) override final;
		};
	}
}