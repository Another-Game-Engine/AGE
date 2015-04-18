#include "BulletPhysics.hpp"
#include "BulletWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		// Inherited Methods
		EngineType BulletPhysics::getPluginType(void) const
		{
			return EngineType::Bullet;
		}

		bool BulletPhysics::initialize(void)
		{
			return true;
		}

		void BulletPhysics::finalize(void)
		{
			return;
		}

		WorldInterface *BulletPhysics::createWorld(const glm::vec3 &gravity)
		{
			return new BulletWorld(this, gravity);
		}
	}
}