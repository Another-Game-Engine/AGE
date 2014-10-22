#pragma once

#include <glm/glm.hpp>

namespace AGE
{
	class Plane
	{
	public:
		Plane(const glm::vec3 &normal = glm::vec3(0, 0, 0), float distance = 0.0f);

		glm::vec3 const &getNormal() const { return _normal; }
		float getDistance() const { return _distance; }
		void setCoefficients(float x, float y, float z, float w);
		float getPointDistance(glm::vec3 const &point) const;

	private:
		void normalize();

		glm::vec3 _normal;
		float _distance;
	};
}