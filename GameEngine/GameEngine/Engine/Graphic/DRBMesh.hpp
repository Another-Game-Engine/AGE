#pragma once

#include <vector>
#include "BFC/BFCCullableObject.hpp"

namespace AGE
{
	struct SubMeshInstance;

	struct DRBMesh : public BFCCullableObject
	{
	public:
		// we have to define an enum
		virtual CullableTypeID getBFCType() const { return 0; }

		std::vector<SubMeshInstance> subMeshs;
		// other stuff plus tard, genre transformation et tout
	};
}