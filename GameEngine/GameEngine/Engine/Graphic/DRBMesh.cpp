#include "DRBMesh.hpp"

namespace AGE
{
	glm::vec4 DRBMesh::setBFCTransform(const glm::mat4 &transformation)
	{
		return BFCCullableObject::setBFCTransform(transformation);
	}
}