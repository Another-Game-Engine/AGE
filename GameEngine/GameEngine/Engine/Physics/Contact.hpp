#pragma once

#include <glm/glm.hpp>

namespace AGE
{
	namespace Physics
	{
		struct Contact final
		{
			glm::vec3 position;
			glm::vec3 normal;
		};
	}
}