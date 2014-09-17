
#include <Utils/AABoundingBox.hh>

#define		MAX_ELEMENT_PER_NODE	4

namespace AGE
{
	enum EOctreeNodes
	{
		NEGX_NEGY_NEGZ = 0,
		POSX_NEGY_NEGZ,
		POSX_POSY_NEGZ,
		POSX_POSY_POSZ,
		POSX_NEGY_POSZ,
		NEGX_POSY_NEGZ,
		NEGX_POSY_POSZ,
		NEGX_NEGY_POSZ,
		NBR_OCTREE_NODES
	};

	class	CullableObject;

	class	OctreeNode
	{
	public:
		OctreeNode();
		~OctreeNode();

		OctreeNode	*AddElement(CullableObject *toAdd);
		OctreeNode	*MoveElement(CullableObject *toAdd);
		OctreeNode	*removeElement(CullableObject *toRm);

	private:
		OctreeNode		*_father;
		OctreeNode		*_sons[NBR_OCTREE_NODES];

		AABoundingBox	_node;

		CullableObject	*_elements[MAX_ELEMENT_PER_NODE];
	};
}