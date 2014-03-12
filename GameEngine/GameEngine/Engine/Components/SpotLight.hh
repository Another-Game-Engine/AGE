#pragma once

#include <Components\Component.hh>
#include <glm\glm.hpp>

struct SpotLightData
{
	glm::mat4		lightVP;
	glm::vec4		positionPower;
	glm::vec4		colorRange;
	unsigned int	hasShadow;
	unsigned int	padding[3];

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
		lightVP = o.lightVP;
		positionPower = o.positionPower;
		colorRange = o.colorRange;
		hasShadow = o.hasShadow;
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
