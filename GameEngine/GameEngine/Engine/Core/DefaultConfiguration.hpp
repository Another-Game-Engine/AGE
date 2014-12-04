#pragma once

#include <functional>

namespace AGE
{
	class Engine;
	std::function<bool()> DefaultEngineConfiguration(Engine *e, std::function<bool()> userConfigs);
}