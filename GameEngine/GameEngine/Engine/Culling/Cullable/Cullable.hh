#pragma once

#include <Culling/PreparableObject.hh>

namespace AGE
{
	class LooseOctreeNode;

	enum CullableType
	{
		CULLABLE_BOUNDING_BOX = 0,
		CULLABLE_SPHERE,
		CULLABLE_FRUSTUM,
		UNKNOWN_CULLABLE
	};

	struct Cullable : public PreparableObject
	{
		CullableType type;
		uint32_t	 currentNode;
		uint32_t	 currentElementInNode;
		uint32_t	 moveBufferIdx;
		Cullable();
		~Cullable();
	};

}