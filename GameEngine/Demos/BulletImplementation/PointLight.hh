#pragma once

#include <Components\Component.hh>
#include <glm\glm.hpp>

struct PointLight : public Component::ComponentBase<PointLight>
{
	glm::vec3	position;
	glm::vec3	color;
	float		range;

	PointLight()
	{
	}

	~PointLight()
	{
	}
};

