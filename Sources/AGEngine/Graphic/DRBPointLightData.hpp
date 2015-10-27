#pragma once

#include "Utils/Debug.hpp"

#include "DRBData.hpp"

#include <glm/glm.hpp>

#include "Render/Properties/AutoProperty.hpp"

#include "Render/ProgramResources/Types/Uniform/Vec4.hh"
#include "Render/ProgramResources/Types/Uniform/Vec3.hh"

namespace AGE
{
	struct DRBPointLightData : public DRBData
	{
		Properties globalProperties;

		DRBPointLightData();
		virtual ~DRBPointLightData();
		void setRange(const glm::vec4 &range);
		void updateRange(const glm::mat4 &transformation);
		// TODO to remove
		virtual void setTransformation(const glm::mat4 &transformation) override;
	private:
		glm::vec4 _range;
		glm::mat4 _sphereTransform;

		std::shared_ptr<AutoProperty<glm::vec3, Vec3>> _positionLightProperty = nullptr;
		std::shared_ptr<AutoProperty<glm::vec4, Vec4>> _rangeProperty = nullptr;
	};
}