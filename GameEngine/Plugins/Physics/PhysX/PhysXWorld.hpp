#pragma once

#include "WorldInterface.hpp"
#include "PhysXPlugin.hpp"

namespace AGE
{
	namespace Physics
	{
		class PhysXWorld final : public WorldInterface
		{
		public:
			// Constructors
			PhysXWorld(void) = delete;

			PhysXWorld(PhysXPlugin *plugin, const glm::vec3 &gravity);

			PhysXWorld(const PhysXWorld &) = delete;

			// Assignment Operators
			PhysXWorld &operator=(const PhysXWorld &) = delete;

			// Destructor
			virtual ~PhysXWorld(void) = default;

		private:
			// Attributes
			physx::PxScene *scene = nullptr;

			// Inherited Methods
			void setGravity(const glm::vec3 &gravity) override final;

			glm::vec3 getGravity(void) const override final;

			void simulate(float stepSize) override final;
		};
	}
}
