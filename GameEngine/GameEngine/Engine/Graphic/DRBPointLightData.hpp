#pragma once

#include "DRBData.hpp"
#include <glm/glm.hpp>

namespace AGE
{
	struct DRBPointLightData : public DRBData
	{
		DRBPointLightData()
			: DRBData()
		{
		}

		virtual ~DRBPointLightData()
		{
		}
		void setRange(const glm::vec3 &range);
		const glm::vec3 getRange() const;
	private:
		glm::vec3 _range;
	};
}