#pragma once

#include <Physics/PhysicsInterface.hpp>

namespace AGE
{
	namespace Physics
	{
		class NullPhysics final : public PhysicsInterface
		{
		public:
			// Constructors
			NullPhysics(void) = default;

			NullPhysics(const NullPhysics &) = delete;

			// Assignment Operators
			NullPhysics &operator=(const NullPhysics &) = delete;

			// Destructor
			virtual ~NullPhysics(void) = default;

		private:
			// Inherited Methods
			EngineType getPluginType(void) const override final;

			bool initialize(void) override final;

			void finalize(void) override final;

			WorldInterface *createWorld(void) override final;
		};
	}
}
