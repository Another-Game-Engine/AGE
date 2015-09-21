#pragma once

#include <glm/glm.hpp>

namespace AGE
{
	namespace Physics
	{
		struct Contact final
		{
			// Attribute
			glm::vec3 position;

			glm::vec3 normal;
		};
	}
}