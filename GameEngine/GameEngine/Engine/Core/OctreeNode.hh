
#include <Utils/AABoundingBox.hh>

#define		MAX_ELEMENT_PER_NODE	4

namespace AGE
{
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
		void		splitNode();
		OctreeNode	*extendNode(CullableObject *toAdd, glm::u8vec3 const &direction);
		bool		isLeaf() const;

		void		generateAllSons();

		OctreeNode		*_father;
		OctreeNode		*_sons[2][2][2];

		AABoundingBox	_node;

		uint32_t		_elementsNbr;
		CullableObject	*_elements[MAX_ELEMENT_PER_NODE];
	};
}