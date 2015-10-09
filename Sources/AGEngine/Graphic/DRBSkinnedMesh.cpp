#include "DRBSkinnedMesh.hpp"
#include "DRBMeshData.hpp"

#include <Render/Properties/SkeletonProperty.hpp>

namespace AGE
{
	DRBSkinnedMesh::DRBSkinnedMesh()
		: _skeletonProperty(nullptr)
	{
		datas->setRenderMode(RenderModes::AGE_SKINNED, true);
	}

	DRBSkinnedMesh::~DRBSkinnedMesh()
	{}

	void DRBSkinnedMesh::setSkinningMatrix(const std::vector<glm::mat4> &skinningMatrix)
	{
		_skeletonProperty->set(skinningMatrix);
	}

}