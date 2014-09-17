
#include <Core/CullableInterfaces.hh>

namespace AGE
{
	CullableObject::CullableObject() :
		type(UNKNOWN_CULLABLE),
		currentNode(NULL)
	{
	}

	CullableObject::~CullableObject()
	{
	}

	CullableBoundingBox::CullableBoundingBox()
	{
		type = CULLABLE_BOUNDING_BOX;
	}

	CullableBoundingBox::~CullableBoundingBox()
	{

	}

	CullableSphere::CullableSphere()
	{
		type = CULLABLE_SPHERE;
	}

	CullableSphere::~CullableSphere()
	{

	}

	CullableFrustum::CullableFrustum()
	{
		type = CULLABLE_FRUSTUM;
	}

	CullableFrustum::~CullableFrustum()
	{

	}

}