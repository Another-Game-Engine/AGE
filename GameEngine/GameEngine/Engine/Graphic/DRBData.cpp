#include "DRBData.hpp"

#include "Render\Properties\Transformation.hh"


namespace AGE
{
	DRBData::DRBData()
	{
		std::shared_ptr<Transformation> transformationProperty = std::make_shared<Transformation>(glm::mat4(1));
		transformationPropertyKey = individualProperties.add_property(transformationProperty);
	}
}