#include "DRBData.hpp"

#include "Render\Properties\Transformation.hh"


namespace AGE
{
	DRBData::DRBData()
	{
	}

	DRBData::~DRBData(){}

	void DRBData::setTransformation(const glm::mat4 &transformation)
	{
		_transformation = transformation;
	}

	glm::mat4 DRBData::getTransformation() const
	{
		return _transformation;
	}
}