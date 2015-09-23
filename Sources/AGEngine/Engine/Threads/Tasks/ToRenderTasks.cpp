#include <Threads/Tasks/ToRenderTasks.hpp>

namespace AGE
{

	namespace Tasks
	{
		Render::CreateRenderContext::CreateRenderContext(Engine *_engine) : engine(_engine)
		{
		}

		Render::InitRenderPipelines::InitRenderPipelines(Engine *_engine) : engine(_engine)
		{
		}

	}

}