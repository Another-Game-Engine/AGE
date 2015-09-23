#pragma once

#include <string>

#include <Physics/EngineType.hpp>

namespace AGE
{
	namespace Physics
	{
		std::string GetPluginNameForEngine(EngineType physicsEngineType);
	}
}