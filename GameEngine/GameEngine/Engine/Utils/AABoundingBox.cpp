
#include <Utils/AABoundingBox.hh>

namespace AGE
{
	AABoundingBox::AABoundingBox()
	{
	}

	AABoundingBox::~AABoundingBox()
	{
	}

	AABoundingBox::AABoundingBox(glm::vec3 const &min, glm::vec3 const &max)
	{
		minPoint = min;
		maxPoint = max;
		center = minPoint + ((maxPoint - minPoint) / 2.0f);
	}

	void AABoundingBox::recomputeCenter()
	{
		center = minPoint + ((maxPoint - minPoint) / 2.0f);
	}

	glm::vec3	AABoundingBox::getPositivePoint(glm::vec3 const &normal) const
	{
		glm::vec3 ret = minPoint;

		if (normal.x >= 0)
			ret.x = maxPoint.x;
		if (normal.y >= 0)
			ret.y = maxPoint.y;
		if (normal.z >= 0)
			ret.z = maxPoint.z;
		return (ret);
	}

	glm::vec3	AABoundingBox::getNegativePoint(glm::vec3 const &normal) const
	{
		glm::vec3 ret = maxPoint;

		if (normal.x >= 0)
			ret.x = minPoint.x;
		if (normal.y >= 0)
			ret.y = minPoint.y;
		if (normal.z >= 0)
			ret.z = minPoint.z;
		return (ret);
	}

	void		AABoundingBox::fromTransformedBox(AABoundingBox const &aabb, glm::mat4 const &transform)
	{
		glm::vec4		boundingBox[8];

		boundingBox[0] = glm::vec4(aabb.minPoint.x, aabb.minPoint.y, aabb.minPoint.z, 1.0f);
		boundingBox[1] = glm::vec4(aabb.maxPoint.x, aabb.minPoint.y, aabb.minPoint.z, 1.0f);
		boundingBox[2] = glm::vec4(aabb.maxPoint.x, aabb.maxPoint.y, aabb.minPoint.z, 1.0f);
		boundingBox[3] = glm::vec4(aabb.maxPoint.x, aabb.maxPoint.y, aabb.maxPoint.z, 1.0f);
		boundingBox[4] = glm::vec4(aabb.maxPoint.x, aabb.minPoint.y, aabb.maxPoint.z, 1.0f);
		boundingBox[5] = glm::vec4(aabb.minPoint.x, aabb.maxPoint.y, aabb.minPoint.z, 1.0f);
		boundingBox[6] = glm::vec4(aabb.minPoint.x, aabb.maxPoint.y, aabb.maxPoint.z, 1.0f);
		boundingBox[7] = glm::vec4(aabb.minPoint.x, aabb.minPoint.y, aabb.maxPoint.z, 1.0f);

		// Transform the bounding box
		for (size_t i = 0; i < 8; ++i)
			boundingBox[i] = transform * boundingBox[i];
		// Find the AA bounding box
		minPoint = glm::vec3(boundingBox[0]);
		maxPoint = glm::vec3(boundingBox[0]);

		for (size_t i = 1; i < 8; ++i)
		{
			minPoint = glm::min(minPoint, glm::vec3(boundingBox[i]));
			maxPoint = glm::max(maxPoint, glm::vec3(boundingBox[i]));
		}
		center = minPoint + ((maxPoint - minPoint) / 2.0f);
	}

	ECollision AABoundingBox::checkCollision(AABoundingBox const &oth, glm::i8vec3 &direction) const
	{
		direction = glm::i8vec3(0);
		if (minPoint.x >= oth.maxPoint.x)
			direction.x = -1;
		if (oth.minPoint.x >= maxPoint.x)
			direction.x = 1;
		if (minPoint.y >= oth.maxPoint.y)
			direction.y = -1;
		if (oth.minPoint.y >= maxPoint.y)
			direction.y = 1;
		if (minPoint.z >= oth.maxPoint.z)
			direction.z = -1;
		if (oth.minPoint.z >= maxPoint.z)
			direction.z = 1;
		if (direction == glm::i8vec3(0))
		{
			if (oth.minPoint.x < minPoint.x)
				direction.x = -1;
			if (oth.maxPoint.x > maxPoint.x)
				direction.x = 1;
			if (oth.minPoint.y < minPoint.y)
				direction.y = -1;
			if (oth.maxPoint.y > maxPoint.y)
				direction.y = 1;
			if (oth.minPoint.z < minPoint.z)
				direction.z = -1;
			if (oth.maxPoint.z > maxPoint.z)
				direction.z = 1;
			if (direction == glm::i8vec3(0))
				return INSIDE;
			else
				return INTERSECT;
		}
		else
			return OUTSIDE;
	}

	bool	AABoundingBox::checkCollision(AABoundingBox const &oth) const
	{
		return (maxPoint.x > oth.minPoint.x &&
			minPoint.x < oth.maxPoint.x &&
			maxPoint.y > oth.minPoint.y &&
			minPoint.y < oth.maxPoint.y &&
			maxPoint.z > oth.minPoint.z &&
			minPoint.z < oth.maxPoint.z);
	}

	bool	AABoundingBox::checkPointIn(glm::vec3 point, glm::i8vec3 &direction) const
	{
		glm::vec3 dif = point - center;
		direction = glm::i8vec3((dif.x >= 0) ? 1 : -1,
			(dif.y >= 0) ? 1 : -1,
			(dif.z >= 0) ? 1 : -1);
		return (point.x >= minPoint.x && point.x <= maxPoint.x &&
			point.y >= minPoint.y && point.y <= maxPoint.y &&
			point.z >= minPoint.z && point.z <= maxPoint.z);
	}

	bool	AABoundingBox::checkPointIn(glm::vec3 point) const
	{
		return (point.x >= minPoint.x && point.x <= maxPoint.x &&
			point.y >= minPoint.y && point.y <= maxPoint.y &&
			point.z >= minPoint.z && point.z <= maxPoint.z);
	}

}