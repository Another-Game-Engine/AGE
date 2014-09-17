
#include <Core/PreparableObject.hh>
#include <Utils/Frustum.hh>
#include <Utils/Sphere.hh>

namespace AGE
{
	class OctreeNode;

	enum CullableType
	{
		CULLABLE_BOUNDING_BOX = 0,
		CULLABLE_SPHERE,
		CULLABLE_FRUSTUM,
		UNKNOWN_CULLABLE
	};

	struct CullableObject : public PreparableObject
	{
		CullableType	type;
		OctreeNode		*currentNode;

		CullableObject();
		~CullableObject();
	};

	struct CullableBoundingBox : public CullableObject
	{
		AABoundingBox		previousAABB;
		AABoundingBox		currentAABB;

		CullableBoundingBox();
		~CullableBoundingBox();
	};

	struct CullableSphere : public CullableObject
	{
		Sphere		previousSphere;
		Sphere		currentSphere;

		CullableSphere();
		~CullableSphere();
	};

	struct CullableFrustum : public CullableObject
	{
		Frustum		previousFrustum;
		Frustum		currentFrustum;

		CullableFrustum();
		~CullableFrustum();
	};

}