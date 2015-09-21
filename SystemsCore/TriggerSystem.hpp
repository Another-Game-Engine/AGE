#pragma once

#include <unordered_set>

#include <System/System.h>
#include <Physics/TriggerListener.hpp>
#include <ComponentsCore/Collider.hpp>
#include <Physics/ITriggerListener.hpp>

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

			// Methods
			void addListener(Physics::ITriggerListener *listener);

			void removeListener(Physics::ITriggerListener *listener);

		private:
			// Attributes
			EntityFilter entityFilter;

			std::unordered_set<Physics::ITriggerListener *> listeners;

			// Inherited Methods
			bool initialize(void) override final;

			void finalize(void) override final;

			void mainUpdate(float elapsedTime) override final;

			void onTrigger(Collider *triggerCollider, Collider *otherCollider, Physics::TriggerType triggerType) override final;
		};
	}
}