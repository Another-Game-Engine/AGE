#include <Physics/Fallback/NullMaterial.hpp>
#include <Physics/Fallback/NullWorld.hpp>

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullMaterial::NullMaterial(ColliderInterface *collider)
			: MaterialInterface(collider)
		{
			return;
		}

		// Inherited Methods
		void NullMaterial::setStaticFriction(float staticFriction)
		{
			return;
		}

		float NullMaterial::getStaticFriction(void) const
		{
			return 0.0f;
		}

		void NullMaterial::setDynamicFriction(float dynamicFriction)
		{
			return;
		}

		float NullMaterial::getDynamicFriction(void) const
		{
			return 0.0f;
		}

		void NullMaterial::setRestitution(float restitution)
		{
			return;
		}

		float NullMaterial::getRestitution(void) const
		{
			return 0.0f;
		}
	}
}