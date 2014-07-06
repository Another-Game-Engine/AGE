#pragma once

#include <Components\Component.hh>
#include <glm\glm.hpp>
#include <cstdint>

struct PointLightData
{
	glm::vec4		positionPower;
	glm::vec4		colorRange;
	std::uint32_t	hasShadow;
	std::uint32_t	padding[3];

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

		virtual void reset(AScene *) { }

		PointLight &operator=(PointLight const &o)
		{
			lightData = o.lightData;
			return (*this);
		}

		void	init(AScene *)
		{
		}

		template <typename Archive>
		void serialize(Archive &ar)
		{
		}

	};

}
