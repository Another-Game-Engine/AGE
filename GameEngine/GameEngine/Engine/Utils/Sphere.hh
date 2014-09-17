
#include <glm/glm.hpp>

namespace AGE
{
	struct	Sphere
	{
		float		radius;
		glm::vec3	center;

		Sphere();
		Sphere(float pRadius, glm::vec3 const &pCenter);
		~Sphere();
	};
}