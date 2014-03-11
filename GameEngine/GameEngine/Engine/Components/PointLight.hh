#pragma once

#include <Components\Component.hh>
#include <glm\glm.hpp>

struct PointLightData
{
	glm::vec4		positionPower;
	glm::vec4		colorRange;
	unsigned int	hasShadow;

	glm::uvec3		padding;

	PointLightData() { }

	PointLightData(PointLightData const &o)
	{
		*this = o;
	}

	~PointLightData()
	{
	}

	PointLightData &operator=(PointLightData const &o)
	{
		positionPower = o.positionPower;
		colorRange = o.colorRange;
		hasShadow = o.hasShadow;
		return (*this);
	}
};

namespace Component
{

	struct PointLight : public ComponentBase<PointLight>
	{
		PointLightData		lightData;

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
