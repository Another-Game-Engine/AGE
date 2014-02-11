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

	PointLight(PointLight const &o)
	{
		*this = o;
	}

	virtual ~PointLight()
	{
	}

	virtual void reset() { }

	PointLight &operator=(PointLight const &o)
	{
		position = o.position;
		color = o.color;
		range = o.range;
		return (*this);
	}

	template <typename Archive>
	Base *unserialize(Archive &ar, Entity e)
	{
		auto res = new PointLight();
		return res;
	}

	template <typename Archive>
	void serialize(Archive &ar)
	{
	}

};

