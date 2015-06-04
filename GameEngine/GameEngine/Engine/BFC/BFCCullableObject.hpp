#pragma once

#include "BFCItemID.hpp"
#include <glm/glm.hpp>

namespace AGE
{
	// all engine cullable and drawable objects
	// inherit from this type
	struct BFCCullableObject
	{
	public:
		virtual ~BFCCullableObject() {}
		virtual CullableTypeID getBFCType() const = 0;
		virtual glm::vec4 setBFCTransform(const glm::mat4 &transformation) { _transform = transformation; return glm::vec4(0, 0, 0, 1); };
	private:
		glm::mat4 _transform;
	};
}