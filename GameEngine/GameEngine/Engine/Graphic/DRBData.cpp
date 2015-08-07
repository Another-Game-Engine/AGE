#include "DRBData.hpp"

#include "Render\Properties\Transformation.hh"


namespace AGE
{
	DRBData::DRBData()
	{
		std::shared_ptr<Transformation> transformationProperty = std::make_shared<Transformation>(glm::mat4(1));
		_transformationPropertyKey = globalProperties.add_property(transformationProperty);
	}

	void DRBData::setTransformation(const glm::mat4 &transformation)
	{
		globalProperties.get_property<Transformation>(_transformationPropertyKey)->set(transformation);
	}

	glm::mat4 DRBData::getTransformation() const
	{
		return globalProperties.get_property<Transformation>(_transformationPropertyKey)->get();
	}
}