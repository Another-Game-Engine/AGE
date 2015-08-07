#pragma once

#include "Render\Properties\Properties.hh"
#include "Utils/RWLock.hpp"
#include "glm/fwd.hpp"

namespace AGE
{
	struct DRBData
	{
		Properties globalProperties;

		DRBData();
		virtual ~DRBData(){}
		void setTransformation(const glm::mat4 &transformation);
		glm::mat4 getTransformation() const;
	protected:
		Key<Property> _transformationPropertyKey;
		mutable RWLock _lock;
	};
}