#pragma once

// We define here or different BFC cullable types
// each type will be managed in a different BFCBlockManager

#include "BFC/BFCItemId.hpp"

namespace AGE
{
	enum BFCCullableType : CullableTypeID
	{
		CullableMesh = 0,
		CullablePointLight,
		CullableSpotLight,
		CullableSkinnedMesh
	};
}