#pragma once

#include "MaterialInterface.hpp"
#include "PhysXWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		class PhysXMaterial final : public MaterialInterface
		{
		public:
			// Constructors
			PhysXMaterial(void) = delete;

			PhysXMaterial(PhysXWorld *world, ColliderInterface *collider);

			PhysXMaterial(const PhysXMaterial &) = delete;

			// Assignment Operators
			PhysXMaterial &operator=(const PhysXMaterial &) = delete;

			// Destructor
			~PhysXMaterial(void) = default;

		private:
			// Attributes
			physx::PxMaterial *material = nullptr;

			// Inherited Methods
			void setStaticFriction(float staticFriction) override final;

			float getStaticFriction(void) const override final;

			void setDynamicFriction(float dynamicFriction) override final;

			float getDynamicFriction(void) const override final;

			void setRestitution(float restitution) override final;

			float getRestitution(void) const override final;
		};
	}
}