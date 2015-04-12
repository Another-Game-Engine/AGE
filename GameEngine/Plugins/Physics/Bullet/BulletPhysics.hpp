#pragma once

#include "PhysicsInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class BulletPhysics final : public PhysicsInterface
		{
		public:
			// Constructors
			BulletPhysics(void) = default;

			BulletPhysics(const BulletPhysics &) = delete;

			// Assignment Operators
			BulletPhysics &operator=(const BulletPhysics &) = delete;

			// Destructor
			virtual ~BulletPhysics(void) = default;

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
