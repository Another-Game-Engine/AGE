#pragma once

#include <Physics/MaterialInterface.hpp>

namespace AGE
{
	namespace Physics
	{
		class NullWorld;

		class NullMaterial final : public MaterialInterface
		{
			// Friendships
			friend ObjectPool < NullMaterial >;

		public:
			// Constructors
			NullMaterial(void) = delete;

			NullMaterial(ColliderInterface *collider);

			NullMaterial(const NullMaterial &) = delete;

			// Assignment Operators
			NullMaterial &operator=(const NullMaterial &) = delete;

		private:
			// Destructor
			~NullMaterial(void) = default;

			// Attributes
			float staticFriction = 0.0f;

			float dynamicFriction = 0.0f;

			float restitution = 0.0f;

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