#pragma once

#include <vector>

#include "BFC/BFCCullableObject.hpp"

#include "BFCCullableTypes.hpp"

#include "AssetManagement\Instance\MeshInstance.hh"

namespace AGE
{
	struct SubMeshInstance;

	struct DRBMesh : public BFCCullableObject
	{
	public:
		virtual CullableTypeID getBFCType() const { return BFCCullableType::CullableMesh; }
		virtual glm::vec4 setBFCTransform(const glm::mat4 &transformation);
		std::vector<SubMeshInstance> subMeshs;
	};
}