
#include <Utils/Sphere.hh>

namespace AGE
{
	Sphere::Sphere()
	{

	}

	Sphere::Sphere(float pRadius, glm::vec3 const &pCenter)
	{
		radius = pRadius;
		center = pCenter;
	}

	Sphere::~Sphere()
	{
		
	}

}