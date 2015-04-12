#pragma once

#include "MaterialInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class BulletWorld;

		class BulletMaterial final : public MaterialInterface
		{
		public:
			// Constructors
			BulletMaterial(void) = default;

			BulletMaterial(const BulletMaterial &) = delete;

			// Assignment Operators
			BulletMaterial &operator=(const BulletMaterial &) = delete;

			// Destructor
			~BulletMaterial(void) = default;

		private:
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