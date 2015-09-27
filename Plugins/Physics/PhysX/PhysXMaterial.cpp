#include "PhysXMaterial.hpp"
#include "PhysXWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		PhysXMaterial::PhysXMaterial(PhysXWorld *world, const std::string &name)
			: MaterialInterface(name), material(static_cast<PhysXPhysics *>(world->getPhysics())->getPhysics()->createMaterial(GetDefaultStaticFriction(), GetDefaultDynamicFriction(), GetDefaultRestitution()))
		{
			assert(material != nullptr && "Impossible to create material");
		}

		// Destructor
		PhysXMaterial::~PhysXMaterial(void)
		{
			if (material != nullptr)
			{
				material->release();
				material = nullptr;
			}
		}

		// Methods
		physx::PxMaterial *PhysXMaterial::getMaterial(void)
		{
			return material;
		}

		const physx::PxMaterial *PhysXMaterial::getMaterial(void) const
		{
			return material;
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