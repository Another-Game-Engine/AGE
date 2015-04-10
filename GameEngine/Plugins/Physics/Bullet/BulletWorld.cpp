#include <cassert>

#include "BulletWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		BulletWorld::BulletWorld(BulletPlugin *plugin, const glm::vec3 &gravity, const std::string &worldName)
			: WorldInterface(worldName), plugin(plugin)
		{
			// TO_DO
		}

		void BulletWorld::setGravity(const glm::vec3 &gravity)
		{
			// TO_DO
		}

		glm::vec3 BulletWorld::getGravity(void) const
		{
			// TO_DO
			return glm::vec3();
		}

		void BulletWorld::simulate(float stepSize)
		{
			// TO_DO
		}
	}
}