
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
		bool			hasBeenFound;

		CullableObject();
		~CullableObject();

		virtual bool	checkCollision(CullableObject *oth, bool thisCurrent = true, bool othCurrent = true) const = 0;
	};

	struct CullableBoundingBox : public CullableObject
	{
		AABoundingBox		previousAABB;
		AABoundingBox		currentAABB;

		CullableBoundingBox();
		~CullableBoundingBox();

		virtual bool	checkCollision(CullableObject *oth, bool thisCurrent = true, bool othCurrent = true) const;
	};

	struct CullableSphere : public CullableObject
	{
		Sphere		previousSphere;
		Sphere		currentSphere;

		CullableSphere();
		~CullableSphere();

		virtual bool	checkCollision(CullableObject *oth, bool thisCurrent = true, bool othCurrent = true) const;
	};

	struct CullableFrustum : public CullableObject
	{
		Frustum		previousFrustum;
		Frustum		currentFrustum;

		CullableFrustum();
		~CullableFrustum();

		virtual bool	checkCollision(CullableObject *oth, bool thisCurrent = true, bool othCurrent = true) const;
	};

}