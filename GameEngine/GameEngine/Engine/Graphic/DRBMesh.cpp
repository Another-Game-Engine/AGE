#include "DRBMesh.hpp"
#include "DRBData.hpp"
#include "DRBMeshData.hpp"

#include "Render\Properties\Transformation.hh"

namespace AGE
{
	DRBMesh::DRBMesh()
	{
		datas = std::make_shared<DRBMeshData>();
	}

	glm::vec4 DRBMesh::setBFCTransform(const glm::mat4 &transformation)
	{
		datas->setTransformation(transformation);
		return BFCCullableObject::setBFCTransform(transformation);
	}

	const std::shared_ptr<DRBData> DRBMesh::getDatas() const
	{
		return std::static_pointer_cast<DRBData>(datas);
	}
}