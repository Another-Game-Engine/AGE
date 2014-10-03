#pragma once

#include <glm/glm.hpp>

namespace AGE
{

	enum ECollision
	{
		INSIDE,
		OUTSIDE,
		INTERSECT
	};

	struct		AABoundingBox
	{
		glm::vec3		minPoint;
		glm::vec3		maxPoint;

		AABoundingBox();
		~AABoundingBox();
		AABoundingBox(glm::vec3 const &min, glm::vec3 const &max);

		void		fromTransformedBox(AABoundingBox const &oth, glm::mat4 const &transform);

		ECollision	checkCollision(AABoundingBox const &oth, glm::u8vec3 &direction) const;
	};
}