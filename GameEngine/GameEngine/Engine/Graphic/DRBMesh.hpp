#pragma once

#include <vector>

#include "BFC/BFCCullableObject.hpp"

#include "BFCCullableTypes.hpp"

#include "AssetManagement\Instance\MeshInstance.hh"

namespace AGE
{
	struct SubMeshInstance;
	struct DRBMeshData;

	struct DRBMesh : public BFCCullableObject
	{
	public:
		DRBMesh();
		virtual CullableTypeID getBFCType() const { return BFCCullableType::CullableMesh; }
		virtual glm::vec4 setBFCTransform(const glm::mat4 &transformation);
		virtual const std::shared_ptr<DRBData> getDatas() const;

		std::shared_ptr<DRBMeshData> datas;
	}; 
}