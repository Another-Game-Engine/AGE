
#include <glm/glm.hpp>

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

	class	Drawable;

	class	OctreeNode
	{
	public:
		OctreeNode();
		~OctreeNode();

		OctreeNode	*AddElement(Drawable *toAdd);
		OctreeNode	*MoveElement(Drawable *toAdd);
		OctreeNode	*removeElement(Drawable *toRm);

	private:
		OctreeNode		*_father;
		OctreeNode		*_sons[NBR_OCTREE_NODES];

		glm::vec3	_minPoint;
		glm::vec3	_maxPoint;

		Drawable	*_elements[MAX_ELEMENT_PER_NODE];
	};
}