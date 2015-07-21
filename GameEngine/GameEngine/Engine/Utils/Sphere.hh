#pragma once

#include <glm/glm.hpp>
#include <Culling/Cullable/Cullable.hh>

namespace AGE
{
	struct	Sphere
	{
		enum { shape_type = CULLABLE_SPHERE };

		float		radius;
		glm::vec3	center;

		Sphere();
		Sphere(float pRadius, glm::vec3 const &pCenter);
		~Sphere();
	};
}