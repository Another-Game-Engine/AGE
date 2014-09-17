#pragma once

#include <glm/glm.hpp>

namespace AGE
{
	struct		AABoundingBox
	{
		glm::vec3		minPoint;
		glm::vec3		maxPoint;

		AABoundingBox();
		~AABoundingBox();
		AABoundingBox(glm::vec3 const &min, glm::vec3 const &max);

		void		fromTransformedBox(AABoundingBox const &oth, glm::mat4 const &transform);
	};
}