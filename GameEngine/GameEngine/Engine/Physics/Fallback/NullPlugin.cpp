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

		WorldInterface *NullPlugin::createWorld(const glm::vec3 &gravity)
		{
			return static_cast<WorldInterface *>(new NullWorld(this));
		}

		void NullPlugin::destroyWorld(WorldInterface *world)
		{
			delete static_cast<NullWorld *>(world);
		}
	}
}