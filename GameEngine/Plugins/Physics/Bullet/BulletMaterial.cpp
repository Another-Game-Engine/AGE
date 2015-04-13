#include "BulletMaterial.hpp"
#include "BulletWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletMaterial::BulletMaterial(ColliderInterface *collider)
			: MaterialInterface(collider)
		{
			return;
		}

		// Inherited Methods
		void BulletMaterial::setStaticFriction(float staticFriction)
		{
			// TO_DO
		}

		float BulletMaterial::getStaticFriction(void) const
		{
			// TO_DO
			return 0.0f;
		}

		void BulletMaterial::setDynamicFriction(float dynamicFriction)
		{
			// TO_DO
		}

		float BulletMaterial::getDynamicFriction(void) const
		{
			// TO_DO
			return 0.0f;
		}

		void BulletMaterial::setRestitution(float restitution)
		{
			// TO_DO
		}

		float BulletMaterial::getRestitution(void) const
		{
			// TO_DO
			return 0.0f;
		}
	}
}