#pragma once

#include <glm/glm.hpp>
#include <TMQ/message.hpp>

namespace AGE
{
	namespace Tasks
	{
		class Render
		{
		public:
			struct GetWindowSize : public TMQ::FutureData < glm::uvec2 >
			{
				GetWindowSize(){}
			};
		};
	}
}