#pragma once

#include <Systems/System.h>
#include <Physics/PhysicsInterface.hpp>

namespace AGE
{
	namespace Private
	{
		class CollisionSystem final : public System
		{
		public:
			// Constructors
			CollisionSystem(void) = delete;

			CollisionSystem(AScene *scene, Physics::PhysicsInterface *physics);

			CollisionSystem(CollisionSystem const &) = delete;

			// Assignment Operators
			CollisionSystem &operator=(CollisionSystem const &) = delete;

			// Destructors
			~CollisionSystem(void) = default;

		private:
			// Attributes
			Physics::PhysicsInterface *physics = nullptr;

			EntityFilter entityFilter;

			// Inherited Methods
			bool initialize(void) override final;

			void finalize(void) override final;

			void mainUpdate(float elapsedTime) override final;
		};
	}
}