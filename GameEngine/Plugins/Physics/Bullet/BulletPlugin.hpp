#pragma once

#include "PhysicsInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class BulletPlugin final : public PhysicsInterface
		{
		public:
			// Constructors
			BulletPlugin(void) = default;

			BulletPlugin(const BulletPlugin &) = delete;

			// Assignment Operators
			BulletPlugin &operator=(const BulletPlugin &) = delete;

			// Destructor
			virtual ~BulletPlugin(void) = default;

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
