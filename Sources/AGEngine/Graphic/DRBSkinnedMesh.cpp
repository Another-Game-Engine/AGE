#include "DRBSkinnedMesh.hpp"
#include "DRBMeshData.hpp"

#include <Render/Properties/SkeletonProperty.hpp>

namespace AGE
{
	DRBSkinnedMesh::DRBSkinnedMesh()
		: DRBMesh(BFCCullableType::CullableSkinnedMesh)
		, _skeletonProperty(nullptr)
	{
		datas->setRenderMode(RenderModes::AGE_SKINNED, true);
	}

	DRBSkinnedMesh::~DRBSkinnedMesh()
	{}

	void DRBSkinnedMesh::setSkinningMatrix(const glm::mat4 *skinningMatrix, std::size_t size)
	{
		_skeletonProperty->set(skinningMatrix, size);
	}

}