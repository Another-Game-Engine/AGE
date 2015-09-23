#pragma once

#include <Utils/Plane.hh>

namespace AGE
{
	Plane::Plane(const glm::vec3 &normal, float distance) :
		_normal(normal),
		_distance(distance)
	{
	}

	void	Plane::setCoefficients(float x, float y, float z, float w)
	{
		_normal = glm::vec3(x, y, z);
		_distance = w;
		normalize();
	}

	float	Plane::getPointDistance(glm::vec3 const &point) const
	{
		return (_normal.x * point.x + _normal.y * point.y + _normal.z * point.z + _distance);
	}

	void	Plane::normalize()
	{
		float divider = 1 / glm::length(_normal);
		_normal = _normal * divider;
		_distance = _distance * divider;
	}

}