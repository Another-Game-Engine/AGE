#pragma once

#include <System/System.h>

namespace AGE
{
	namespace Private
	{
		class PhysicsDebugSystem final : public System<PhysicsDebugSystem>
		{
		public:
			// Constructors
			PhysicsDebugSystem(void) = delete;

			PhysicsDebugSystem(AScene *scene);

			PhysicsDebugSystem(PhysicsDebugSystem const &) = delete;

			PhysicsDebugSystem(PhysicsDebugSystem &&) = delete;

			// Assignment Operators
			PhysicsDebugSystem &operator=(PhysicsDebugSystem const &) = delete;

			PhysicsDebugSystem &operator=(PhysicsDebugSystem &&) = delete;

			// Destructors
			~PhysicsDebugSystem(void) = default;

		private:
			// Inherited Methods
			bool initialize(void) override final;

			void finalize(void) override final;

			void mainUpdate(float elapsedTime) override final;
		};
	}
}