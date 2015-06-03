#pragma once

#include <vector>

namespace AGE
{
	struct SubMeshInstance;

	struct DRBMesh
	{
		std::vector<SubMeshInstance> subMeshs;
		// other stuff plus tard, genre transformation et tout
	};
}