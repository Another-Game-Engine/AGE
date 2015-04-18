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
			setStaticFriction(GetDefaultStaticFriction());
			setDynamicFriction(GetDefaultDynamicFriction());
			setRestitution(GetDefaultRestitution());
		}

		// Inherited Methods
		void BulletMaterial::setStaticFriction(float staticFriction)
		{
			material.m_friction = staticFriction;
		}

		float BulletMaterial::getStaticFriction(void) const
		{
			return material.m_friction;
		}

		void BulletMaterial::setDynamicFriction(float dynamicFriction)
		{
			material.m_friction = dynamicFriction;
		}

		float BulletMaterial::getDynamicFriction(void) const
		{
			return material.m_friction;
		}

		void BulletMaterial::setRestitution(float restitution)
		{
			material.m_restitution = restitution;
		}

		float BulletMaterial::getRestitution(void) const
		{
			return material.m_restitution;
		}
	}
}