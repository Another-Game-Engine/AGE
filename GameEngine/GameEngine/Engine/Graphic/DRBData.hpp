#pragma once

#include "Render\Properties\Properties.hh"

namespace AGE
{
	struct DRBData
	{
		Key<Property> transformationPropertyKey;

		Properties globalProperties;
		Properties individualProperties;

		DRBData();
		virtual ~DRBData(){}
	};
}