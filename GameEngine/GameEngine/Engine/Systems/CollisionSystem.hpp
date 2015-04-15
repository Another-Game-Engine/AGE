#pragma once

#include <Systems/System.h>
#include <Physics/CollisionListener.hpp>
#include <Components/Collider.hpp>

namespace AGE
{
	namespace Private
	{
		class CollisionSystem final : public System, public Physics::CollisionListener
		{
		public:
			// Constructors
			CollisionSystem(void) = delete;

			CollisionSystem(AScene *scene);

			CollisionSystem(CollisionSystem const &) = delete;

			// Assignment Operators
			CollisionSystem &operator=(CollisionSystem const &) = delete;

			// Destructors
			~CollisionSystem(void) = default;

		private:
			// Attributes
			EntityFilter entityFilter;

			// Inherited Methods
			bool initialize(void) override final;

			void finalize(void) override final;

			void mainUpdate(float elapsedTime) override final;

			void onCollision(Collider *currentCollider, Collider *hitCollider, std::vector<Physics::Contact> contacts, Physics::ContactType contactType) override final;
		};
	}
}