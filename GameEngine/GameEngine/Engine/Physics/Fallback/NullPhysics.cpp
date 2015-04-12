#include "NullPhysics.hpp"
#include "NullWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		// Inherited Methods
		EngineType NullPhysics::getPluginType(void) const
		{
			return EngineType::Null;
		}

		bool NullPhysics::initialize(void)
		{
			return true;
		}

		void NullPhysics::finalize(void)
		{
			return;
		}

		WorldInterface *NullPhysics::createWorld(const glm::vec3 &gravity)
		{
			return static_cast<WorldInterface *>(new NullWorld(this));
		}
	}
}