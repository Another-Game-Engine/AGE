#include <Threads/Tasks/ToRenderTasks.hpp>
#include <Engine/Render/Properties/Properties.hh>
#include <glm/glm.hpp>

namespace AGE
{

	namespace Tasks
	{
		Render::GetWindowSize::GetWindowSize()
		{
		}
		
		Render::SetWindowSize::SetWindowSize(const glm::uvec2 & _size)
			: size(_size)
		{
		}

		Render::CreateRenderContext::CreateRenderContext(Engine *_engine) : engine(_engine)
		{
		}

	}

}