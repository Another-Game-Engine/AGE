#pragma once

#include <Utils/ThreadQueue.hpp>
#include <Utils/Dependency.hpp>

namespace AGE
{
	class RenderThreadInterface : public ThreadQueue, public Dependency < RenderThreadInterface >
	{
	};
}