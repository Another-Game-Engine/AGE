#pragma once

#include "Render\Properties\Properties.hh"
#include "Utils/RWLock.hpp"

namespace AGE
{
	struct DRBData
	{
		Key<Property> transformationPropertyKey;

		Properties globalProperties;
		Properties individualProperties;

		DRBData();
		virtual ~DRBData(){}

	protected:
		mutable RWLock _lock;
	};
}