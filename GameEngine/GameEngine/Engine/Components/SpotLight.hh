#pragma once

#include <Components/Component.hh>
#include <glm/fwd.hpp>

namespace AGE
{
	struct SpotLightData
	{
		glm::mat4		lightVP;
		glm::vec4		positionPower;
		glm::vec4		colorRange;
		int				shadowId;
		unsigned int	padding[3];

		SpotLightData();
		SpotLightData(SpotLightData const &o) = delete;
		~SpotLightData();
		SpotLightData &operator=(SpotLightData const &o) = delete;
	};

	struct SpotLight : public ComponentBase
	{
		SpotLightData		lightData;
		glm::mat4			projection;

		SpotLight();

		SpotLight(SpotLight const &o);

		virtual ~SpotLight();

		void		updateLightData(const glm::mat4 &globalTransform);

		virtual void reset();

		SpotLight &operator=(SpotLight const &o);

		void	init();

		template <typename Archive>
		void serialize(Archive &ar);

	};
}