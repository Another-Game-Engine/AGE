#pragma once

#include <Components\Component.hh>
#include <glm\glm.hpp>

struct SpotLightData
{
	glm::vec4	positionPower;
	glm::vec4	colorRange;

	SpotLightData() { }

	SpotLightData(SpotLightData const &o)
	{
		*this = o;
	}

	~SpotLightData()
	{
	}

	SpotLightData &operator=(SpotLightData const &o)
	{
		positionPower = o.positionPower;
		colorRange = o.colorRange;
		return (*this);
	}
};

namespace Component
{

	struct SpotLight : public ComponentBase<SpotLight>
	{
		SpotLightData		lightData;

		SpotLight()
		{
		}

		SpotLight(SpotLight const &o)
		{
			*this = o;
		}

		virtual ~SpotLight()
		{
		}

		virtual void reset() { }

		SpotLight &operator=(SpotLight const &o)
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
