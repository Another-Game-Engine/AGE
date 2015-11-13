#pragma once

#include "BFC/BFCCullableObject.hpp"

#include "BFCCullableTypes.hpp"

#include "AssetManagement\Instance\MeshInstance.hh"

namespace AGE
{
	struct SubMeshInstance;
	struct DRBMeshData;
	struct MaterialInstance;

	struct DRBMesh : public BFCCullableObject
	{
	protected:
		DRBMesh(CullableTypeID type);
	public:
		DRBMesh();
		virtual glm::vec4 setBFCTransform(const glm::mat4 &transformation);
		const std::shared_ptr<DRBMeshData> getDatas() const;

		MaterialInstance *material;
		std::shared_ptr<DRBMeshData> datas;
	}; 
}