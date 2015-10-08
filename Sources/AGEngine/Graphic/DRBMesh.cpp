#include "DRBMesh.hpp"
#include "DRBData.hpp"
#include "DRBMeshData.hpp"

#include "Render\Properties\Transformation.hh"

namespace AGE
{
	DRBMesh::DRBMesh()
		: BFCCullableObject(BFCCullableType::CullableMesh)
	{
		datas = std::make_shared<DRBMeshData>();
	}

	glm::vec4 DRBMesh::setBFCTransform(const glm::mat4 &transformation)
	{
		datas->setTransformation(transformation);
		return BFCCullableObject::setBFCTransform(transformation);
	}

	const std::shared_ptr<DRBMeshData> DRBMesh::getDatas() const
	{
		return datas;
	}
}