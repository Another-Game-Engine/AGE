#pragma once

#include <Components\Component.hh>
#include <glm\glm.hpp>

struct ContiguousLight
{
	glm::vec4	position;
	glm::vec4	colorRange;

	ContiguousLight() { }

	ContiguousLight(ContiguousLight const &o)
	{
		*this = o;
	}

	~ContiguousLight()
	{
	}

	ContiguousLight &operator=(ContiguousLight const &o)
	{
		position = o.position;
		colorRange = o.colorRange;
		return (*this);
	}
};

namespace Component
{

	struct PointLight : public ComponentBase<PointLight>
	{
		ContiguousLight		lightData;

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
			lightData = o.lightData;
			return (*this);
		}

		void	init()
		{
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

}
