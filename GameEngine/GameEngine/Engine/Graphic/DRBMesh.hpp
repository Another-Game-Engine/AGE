#pragma once

#include <vector>

#include "BFC/BFCCullableObject.hpp"

#include "BFCCullableTypes.hpp"

namespace AGE
{
	struct SubMeshInstance;
	struct MaterialInstance;

	struct DRBMesh : public BFCCullableObject
	{
	public:
		virtual CullableTypeID getBFCType() const { return BFCCullableType::CullableMesh; }

		std::vector<SubMeshInstance> subMeshs;
		std::vector<MaterialInstance> subMaterials;
	};
}