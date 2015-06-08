#pragma once

#include "BFCItemID.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace AGE
{
	struct DRBData;

	// all engine cullable and drawable objects
	// inherit from this type
	struct BFCCullableObject
	{
	public:
		virtual ~BFCCullableObject() {}
		virtual CullableTypeID getBFCType() const = 0;
		virtual glm::vec4 setBFCTransform(const glm::mat4 &transformation) { _transform = transformation; return glm::vec4(0, 0, 0, 1); };
		virtual const std::shared_ptr<DRBData> getDatas() const = 0;
	private:
		glm::mat4 _transform;
	};
}