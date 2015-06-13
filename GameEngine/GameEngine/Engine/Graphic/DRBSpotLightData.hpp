#pragma once

#include <glm/fwd.hpp>

#include "DRBData.hpp"

#include "Render/Properties/AutoProperty.hpp"

namespace AGE
{
	class Vec3;

	struct DRBSpotLightData : public DRBData
	{
		DRBSpotLightData()
			: DRBData()
		{
		}

		virtual ~DRBSpotLightData()
		{
		}

		// Call that at init, it will add the property in Properties list
		// and keep a pointer on it to update it when spotlight transformation is modified
		void registerDirectionProperty(std::shared_ptr<AutoProperty<glm::vec3, Vec3>> directionProperty);

		// Call that at init, it will add the property in Properties list
		// and keep a pointer on it to update it when spotlight transformation is modified
		void registerPositionProperty(std::shared_ptr<AutoProperty<glm::vec3, Vec3>> positionProperty);

		void computePositionAndDirection(const glm::mat4 &transformation);
	private:
		std::shared_ptr<AutoProperty<glm::vec3, Vec3>> _directionProperty = nullptr;
		std::shared_ptr<AutoProperty<glm::vec3, Vec3>> _positionProperty = nullptr;
	};
}