#include "NullPlugin.hpp"
#include "NullWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		// Inherited Methods
		EngineTypes NullPlugin::getPluginType(void) const
		{
			return EngineTypes::Null;
		}

		bool NullPlugin::initialize(void)
		{
			return true;
		}

		void NullPlugin::finalize(void)
		{
			return;
		}

		WorldInterface *NullPlugin::createWorld(const glm::vec3 &gravity, const std::string &worldName)
		{
			static NullWorld world(worldName);
			return static_cast<WorldInterface *>(&world);
		}

		void NullPlugin::deleteWorld(WorldInterface *world)
		{
			delete static_cast<NullWorld *>(world);
		}
	}
}