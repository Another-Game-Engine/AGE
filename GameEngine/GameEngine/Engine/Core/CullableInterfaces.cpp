
#include <Core/CullableInterfaces.hh>

namespace AGE
{
	CullableObject::CullableObject() :
		type(UNKNOWN_CULLABLE),
		currentNode(NULL),
		hasBeenFound(false)
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

	bool	CullableBoundingBox::checkCollision(CullableObject *oth, bool thisCurrent, bool othCurrent) const
	{
		switch (oth->type)
		{
		case CULLABLE_FRUSTUM:
			{
				AABoundingBox const *toTest = (thisCurrent ? &currentAABB : &previousAABB);
				Frustum const *tested = (othCurrent ?
										&((CullableFrustum*)oth)->currentFrustum :
										&((CullableFrustum*)oth)->previousFrustum);
				return (tested->checkCollision(*toTest));
			}
			break;
		case CULLABLE_BOUNDING_BOX:
			{
				AABoundingBox const *toTest = (thisCurrent ? &currentAABB : &previousAABB);
				AABoundingBox const *tested = (othCurrent ? 
											&((CullableBoundingBox*)oth)->currentAABB :
											&((CullableBoundingBox*)oth)->previousAABB);
				return (toTest->checkCollision(*tested));
			}
			break;
		default:
			assert(!"CullableBoundingBox does not handle collision with this type of cullable.");
			return (false);
			break;
		}
	}

	CullableSphere::CullableSphere()
	{
		type = CULLABLE_SPHERE;
	}

	CullableSphere::~CullableSphere()
	{

	}

	bool	CullableSphere::checkCollision(CullableObject *oth, bool thisCurrent, bool othCurrent) const
	{
		switch (oth->type)
		{
		default:
			assert(!"CullableBoundingBox does not handle collision with this type of cullable.");
			return (false);
			break;
		}
	}

	CullableFrustum::CullableFrustum()
	{
		type = CULLABLE_FRUSTUM;
	}

	CullableFrustum::~CullableFrustum()
	{

	}

	bool	CullableFrustum::checkCollision(CullableObject *oth, bool thisCurrent, bool othCurrent) const
	{
		switch (oth->type)
		{
		case CULLABLE_BOUNDING_BOX:
			{
				Frustum const *toTest = (thisCurrent ? &currentFrustum : &previousFrustum);
				AABoundingBox const *tested = (othCurrent ?
											&((CullableBoundingBox*)oth)->currentAABB :
											&((CullableBoundingBox*)oth)->previousAABB);
				return (toTest->checkCollision(*tested));
			}
			break;
		default:
			assert(!"CullableBoundingBox does not handle collision with this type of cullable.");
			return (false);
			break;
		}
	}

}