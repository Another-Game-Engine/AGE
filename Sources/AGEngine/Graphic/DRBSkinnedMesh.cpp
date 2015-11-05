#include "DRBSkinnedMesh.hpp"
#include "DRBMeshData.hpp"

namespace AGE
{
	DRBSkinnedMesh::DRBSkinnedMesh()
		: DRBMesh(BFCCullableType::CullableSkinnedMesh)
	{
		datas->setRenderMode(RenderModes::AGE_SKINNED, true);
	}

	DRBSkinnedMesh::~DRBSkinnedMesh()
	{}

	void DRBSkinnedMesh::setSkinningMatrix(std::size_t size)
	{
		_skinningMatrixIndex = size;
	}

}