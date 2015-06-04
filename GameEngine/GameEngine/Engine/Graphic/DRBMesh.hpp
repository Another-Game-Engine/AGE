#pragma once

#include <vector>
#include "BFC/BFCCullableObject.hpp"

namespace AGE
{
	struct SubMeshInstance;

	struct DRBMesh : public BFCCullableObject
	{
		std::vector<SubMeshInstance> subMeshs;
		// other stuff plus tard, genre transformation et tout
	};
}