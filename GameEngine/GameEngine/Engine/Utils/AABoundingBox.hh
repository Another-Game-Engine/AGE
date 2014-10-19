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

		glm::vec3	getPositivePoint(glm::vec3 const &normal) const;
		glm::vec3	getNegativePoint(glm::vec3 const &normal) const;

		// create an AABB from another AABB and a transformation matrix
		void		fromTransformedBox(AABoundingBox const &oth, glm::mat4 const &transform);

		// check the state of collision between two AABB
		ECollision checkCollision(AABoundingBox const &oth, glm::i8vec3 &direction) const;

		bool		checkCollision(AABoundingBox const &oth) const;

		template <class Archive>
		void serialize(Archive &ar)
		{
			ar(minPoint, maxPoint);
		}
	};
}