#pragma once

#include "BFCItemID.hpp"
#include <glm/glm.hpp>
#include <memory>

#include <Utils/MatrixConversion.hpp>

#include <glm/gtx/component_wise.hpp>

namespace AGE
{
	struct DRBData;

	// all engine cullable and drawable objects
	// inherit from this type
	struct BFCCullableObject
	{
	public:
		BFCCullableObject(CullableTypeID type) : _type(type) {}
		virtual ~BFCCullableObject() {}
		inline CullableTypeID getBFCType() const { return _type; };
		virtual glm::vec4 setBFCTransform(const glm::mat4 &transformation);
	protected:
		glm::mat4 _transform;
		const CullableTypeID _type;
	};
}