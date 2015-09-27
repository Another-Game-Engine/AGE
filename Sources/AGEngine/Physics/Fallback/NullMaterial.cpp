#include <Physics/Fallback/NullMaterial.hpp>
#include <Physics/Fallback/NullWorld.hpp>

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullMaterial::NullMaterial(const std::string &name)
			: MaterialInterface(name), staticFriction(GetDefaultStaticFriction()), dynamicFriction(GetDefaultDynamicFriction()), restitution(GetDefaultRestitution())
		{
			return;
		}

		// Inherited Methods
		void NullMaterial::setStaticFriction(float staticFriction)
		{
			this->staticFriction = staticFriction;
		}

		float NullMaterial::getStaticFriction(void) const
		{
			return staticFriction;
		}

		void NullMaterial::setDynamicFriction(float dynamicFriction)
		{
			this->dynamicFriction = dynamicFriction;
		}

		float NullMaterial::getDynamicFriction(void) const
		{
			return dynamicFriction;
		}

		void NullMaterial::setRestitution(float restitution)
		{
			this->restitution = restitution;
		}

		float NullMaterial::getRestitution(void) const
		{
			return restitution;
		}
	}
}