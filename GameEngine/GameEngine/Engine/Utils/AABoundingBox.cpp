
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
	}
}