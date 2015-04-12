#include "BulletPhysics.hpp"
#include "BulletWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		// Inherited Methods
		EngineTypes BulletPhysics::getPluginType(void) const
		{
			return EngineTypes::Bullet;
		}

		bool BulletPhysics::initialize(void)
		{
			// TO_DO
			return true;
		}

		void BulletPhysics::finalize(void)
		{
			// TO_DO
		}

		WorldInterface *BulletPhysics::createWorld(const glm::vec3 &gravity)
		{
			return static_cast<WorldInterface *>(new BulletWorld(this, gravity));
		}

		void BulletPhysics::destroyWorld(WorldInterface *world)
		{
			delete static_cast<BulletWorld *>(world);
		}
	}
}