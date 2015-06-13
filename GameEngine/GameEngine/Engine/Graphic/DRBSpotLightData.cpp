#include "DRBSpotLightData.hpp"

#include "Render/ProgramResources/Types/Uniform/Vec3.hh"

namespace AGE
{
	// Call that at init, it will add the property in Properties list
	// and keep a pointer on it to update it when spotlight transformation is modified
	void DRBSpotLightData::registerDirectionProperty(std::shared_ptr<AutoProperty<glm::vec3, Vec3>> directionProperty)
	{
		AGE_ASSERT(_directionProperty == nullptr);
		_directionProperty = directionProperty;
	}

	// Call that at init, it will add the property in Properties list
	// and keep a pointer on it to update it when spotlight transformation is modified
	void DRBSpotLightData::registerPositionProperty(std::shared_ptr<AutoProperty<glm::vec3, Vec3>> positionProperty)
	{
		AGE_ASSERT(_positionProperty == nullptr);
		_positionProperty = positionProperty;
	}

	void DRBSpotLightData::computePositionAndDirection(const glm::mat4 &transformation)
	{
		auto position = glm::vec3(transformation[3]);
		auto direction = glm::transpose(glm::inverse(glm::mat3(transformation))) * glm::vec3(0.0f, 0.0f, -1.0f);
		_positionProperty->set(position);
		_directionProperty->set(direction);
	}
}