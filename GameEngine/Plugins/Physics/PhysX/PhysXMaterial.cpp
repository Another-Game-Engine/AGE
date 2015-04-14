#include "PhysXMaterial.hpp"
#include "PhysXWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		PhysXMaterial::PhysXMaterial(PhysXWorld *world, ColliderInterface *collider)
			: MaterialInterface(collider), material(static_cast<PhysXPhysics *>(world->getPhysics())->getPhysics()->createMaterial(0.5f, 0.5f, 0.1f))
		{
			assert(material != nullptr && "Impossible to create material");
		}

		// Inherited Methods
		void PhysXMaterial::setStaticFriction(float staticFriction)
		{
			material->setStaticFriction(staticFriction);
		}

		float PhysXMaterial::getStaticFriction(void) const
		{
			return material->getStaticFriction();
		}

		void PhysXMaterial::setDynamicFriction(float dynamicFriction)
		{
			material->setDynamicFriction(dynamicFriction);
		}

		float PhysXMaterial::getDynamicFriction(void) const
		{
			return material->getDynamicFriction();
		}

		void PhysXMaterial::setRestitution(float restitution)
		{
			material->setRestitution(restitution);
		}

		float PhysXMaterial::getRestitution(void) const
		{
			return material->getRestitution();
		}
	}
}