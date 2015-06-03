#pragma once

#include <vector>
#include "BFC/BFCRootType.hpp"

namespace AGE
{
	struct SubMeshInstance;

	struct DRBMesh : public BFCRootType
	{
		std::vector<SubMeshInstance> subMeshs;
		// other stuff plus tard, genre transformation et tout
	};
}