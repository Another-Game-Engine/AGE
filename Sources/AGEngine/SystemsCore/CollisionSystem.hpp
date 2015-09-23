#pragma once

#include <unordered_set>

#include <System/System.h>
#include <Physics/CollisionListener.hpp>
#include <ComponentsCore/Collider.hpp>
#include <Physics/ICollisionListener.hpp>

namespace AGE
{
	namespace Private
	{
		class CollisionSystem final : public System<CollisionSystem>, public Physics::CollisionListener
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

			// Methods
			void addListener(Physics::ICollisionListener *listener);
			
			void removeListener(Physics::ICollisionListener *listener);

		private:
			// Attributes
			EntityFilter entityFilter;
			
			std::unordered_set<Physics::ICollisionListener *> listeners;

			// Inherited Methods
			bool initialize(void) override final;

			void finalize(void) override final;

			void mainUpdate(float elapsedTime) override final;

			void onCollision(Collider *currentCollider, Collider *hitCollider, const std::vector<Physics::Contact> &contacts, Physics::CollisionType CollisionType) override final;
		};
	}
}