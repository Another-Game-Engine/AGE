#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <TMQ/message.hpp>

namespace AGE
{
	class Engine;
	struct DrawableCollection;

	namespace Tasks
	{
		class Render
		{
		public:
			struct GetWindowSize : public TMQ::FutureData < glm::uvec2 >
			{
				GetWindowSize();
			};

			struct SetWindowSize
			{
				SetWindowSize(const glm::uvec2 & _size);
				glm::uvec2 size;
			};

			struct CreateRenderContext : TMQ::FutureData < bool >
			{
				CreateRenderContext(std::weak_ptr<Engine> _engine);
				std::weak_ptr<Engine> engine;
			};
		};
	}
}