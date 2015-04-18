#pragma once

#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionShapes/btMaterial.h>

#include "MaterialInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class BulletWorld;

		class BulletMaterial final : public MaterialInterface
		{
			// Friendships
			friend ObjectPool < BulletMaterial > ;

		public:
			// Constructors
			BulletMaterial(void) = delete;

			BulletMaterial(ColliderInterface *collider);

			BulletMaterial(const BulletMaterial &) = delete;

			// Assignment Operators
			BulletMaterial &operator=(const BulletMaterial &) = delete;

		private:
			// Attributes
			btMaterial material;

			// Destructor
			~BulletMaterial(void) = default;

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