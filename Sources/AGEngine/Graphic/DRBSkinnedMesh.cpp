#include "DRBSkinnedMesh.hpp"
#include "DRBMeshData.hpp"

namespace AGE
{
	DRBSkinnedMesh::DRBSkinnedMesh()
		: _skeletonProperty(nullptr)
	{
		datas->setRenderMode(RenderModes::AGE_SKINNED, true);
	}

	DRBSkinnedMesh::~DRBSkinnedMesh()
	{}
}