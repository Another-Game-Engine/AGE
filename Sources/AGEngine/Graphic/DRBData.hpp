#pragma once

#include "glm/glm.hpp"

namespace AGE
{
	struct DRBData
	{
		DRBData();
		virtual ~DRBData();
		//TODO remove virtual when removing properties from pointlights
		void setTransformation(const glm::mat4 &transformation);
		glm::mat4 getTransformation() const;
	protected:
		glm::mat4 _transformation;
	};
}