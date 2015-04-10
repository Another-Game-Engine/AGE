#pragma once

#include <Physics/PhysicsInterface.hpp>

namespace AGE
{
	namespace Physics
	{
		class NullPlugin final : public PhysicsInterface
		{
		public:
			// Constructors
			NullPlugin(void) = default;

			NullPlugin(const NullPlugin &) = delete;

			// Assignment Operators
			NullPlugin &operator=(const NullPlugin &) = delete;

			// Destructor
			virtual ~NullPlugin(void) = default;

		private:
			// Inherited Methods
			EngineTypes getPluginType(void) const override final;

			bool initialize(void) override final;

			void finalize(void) override final;

			WorldInterface *createWorld(const glm::vec3 &gravity) override final;

			void destroyWorld(WorldInterface *world) override final;
		};
	}
}
