#pragma once

#include <Physics/MaterialInterface.hpp>

namespace AGE
{
	namespace Physics
	{
		class NullWorld;

		class NullMaterial final : public MaterialInterface
		{
		public:
			// Constructors
			NullMaterial(void) = default;

			NullMaterial(const NullMaterial &) = delete;

			// Assignment Operators
			NullMaterial &operator=(const NullMaterial &) = delete;

			// Destructor
			~NullMaterial(void) = default;

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