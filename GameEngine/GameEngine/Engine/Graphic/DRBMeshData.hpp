#pragma once

#include "DRBData.hpp"

namespace AGE
{
	struct DRBMeshData : public DRBData
	{
		DRBMeshData()
			: DRBData()
		{
		}

		virtual ~DRBMeshData()
		{
		}
		Key<Painter> painter;
		Key<Vertices> vertices;
	};
}