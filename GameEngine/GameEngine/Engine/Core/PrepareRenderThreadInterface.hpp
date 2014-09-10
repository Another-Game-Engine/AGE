#pragma once

#include <Utils/ThreadQueue.hpp>
#include <Utils/Dependency.hpp>

namespace AGE
{
	class PrepareRenderThreadInterface : public ThreadQueue, public Dependency < PrepareRenderThreadInterface >
	{
		
	};

	namespace Threads
	{
		typedef PrepareRenderThread Prepare;
	}
}