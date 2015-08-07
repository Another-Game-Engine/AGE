#include "DRBSpotLightData.hpp"

#include "Render/ProgramResources/Types/Uniform/Vec3.hh"
#include "Render/ProgramResources/Types/Uniform/Mat4.hh"


namespace AGE
{
	// Call that at init, it will add the property in Properties list
	// and keep a pointer on it to update it when spotlight transformation is modified
	void DRBSpotLightData::registerDirectionProperty(std::shared_ptr<AutoProperty<glm::vec3, Vec3>> directionProperty)
	{
		AGE_ASSERT(_directionProperty == nullptr);
		_directionProperty = directionProperty;
		globalProperties.add_property(_directionProperty);
	}

	// Call that at init, it will add the property in Properties list
	// and keep a pointer on it to update it when spotlight transformation is modified
	void DRBSpotLightData::registerPositionProperty(std::shared_ptr<AutoProperty<glm::vec3, Vec3>> positionProperty)
	{
		AGE_ASSERT(_positionProperty == nullptr);
		_positionProperty = positionProperty;
		globalProperties.add_property(_directionProperty);
	}

	// Call that at init, it will add the property in Properties list
	// and keep a pointer on it 
	void DRBSpotLightData::registerShadowMatrixProperty(std::shared_ptr<AutoProperty<glm::mat4, Mat4>> shadowMatrixProperty)
	{
		AGE_ASSERT(_shadowMatrixProperty == nullptr);
		_shadowMatrixProperty = shadowMatrixProperty;
		globalProperties.add_property(_shadowMatrixProperty);
	}

	void DRBSpotLightData::computePositionAndDirection(const glm::mat4 &transformation)
	{
		auto position = glm::vec3(transformation[3]);
		auto direction = glm::transpose(glm::inverse(glm::mat3(transformation))) * glm::vec3(0.0f, 0.0f, -1.0f);
		_positionProperty->set(position);
		_directionProperty->set(direction);
	}
}