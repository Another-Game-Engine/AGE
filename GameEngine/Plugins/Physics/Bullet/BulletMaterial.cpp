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
			assert(material != nullptr && "Impossible to create material");
		}

		// Destructor
		BulletMaterial::~BulletMaterial(void)
		{
			delete material;
			material = nullptr;
		}

		// Inherited Methods
		void BulletMaterial::setStaticFriction(float staticFriction)
		{
			assert(material != nullptr && "Invalid material");
			material->m_friction = staticFriction;
		}

		float BulletMaterial::getStaticFriction(void) const
		{
			assert(material != nullptr && "Invalid material");
			return material->m_friction;
		}

		void BulletMaterial::setDynamicFriction(float dynamicFriction)
		{
			assert(material != nullptr && "Invalid material");
			material->m_friction = dynamicFriction;
		}

		float BulletMaterial::getDynamicFriction(void) const
		{
			assert(material != nullptr && "Invalid material");
			return material->m_friction;
		}

		void BulletMaterial::setRestitution(float restitution)
		{
			assert(material != nullptr && "Invalid material");
			material->m_restitution = restitution;
		}

		float BulletMaterial::getRestitution(void) const
		{
			assert(material != nullptr && "Invalid material");
			return material->m_restitution;
		}
	}
}