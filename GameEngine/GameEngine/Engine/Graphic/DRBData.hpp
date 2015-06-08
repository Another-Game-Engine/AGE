#pragma once

#include "Render\Properties\Properties.hh"
#include "Utils/RWLock.hpp"

namespace AGE
{
	struct DRBData
	{
		Key<Property> transformationPropertyKey;

		Properties globalProperties;

		DRBData();
		virtual ~DRBData(){}

	protected:
		mutable RWLock _lock;
	};
}