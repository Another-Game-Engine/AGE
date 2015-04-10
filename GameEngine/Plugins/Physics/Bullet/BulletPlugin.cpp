#include "BulletPlugin.hpp"
#include "BulletWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		// Inherited Methods
		EngineTypes BulletPlugin::getPluginType(void) const
		{
			return EngineTypes::Bullet;
		}

		bool BulletPlugin::initialize(void)
		{
			// TO_DO
			return true;
		}

		void BulletPlugin::finalize(void)
		{
			// TO_DO
		}

		WorldInterface *BulletPlugin::createWorld(const glm::vec3 &gravity)
		{
			return static_cast<WorldInterface *>(new BulletWorld(this, gravity));
		}

		void BulletPlugin::destroyWorld(WorldInterface *world)
		{
			delete static_cast<BulletWorld *>(world);
		}
	}
}