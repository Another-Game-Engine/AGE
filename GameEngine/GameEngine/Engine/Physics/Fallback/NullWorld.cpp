#include "NullWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullWorld::NullWorld(const std::string &worldName)
			: WorldInterface(worldName)
		{
			return;
		}

		// Inherited Methods
		void NullWorld::setGravity(const glm::vec3 &gravity)
		{
			return;
		}

		glm::vec3 NullWorld::getGravity(void) const
		{
			return glm::vec3();
		}

		void NullWorld::simulate(float stepSize)
		{
			return;
		}
	}
}