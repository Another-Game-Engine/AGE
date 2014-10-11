
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

		OctreeNode	*addElement(CullableObject *toAdd);
		OctreeNode	*moveElement(CullableObject *toAdd);
		OctreeNode	*removeElement(CullableObject *toRm);

		AABoundingBox const &getNodeBoundingBox() const;
		bool				isLeaf() const;

	private:
		void		splitNode();
		OctreeNode *extendNode(CullableObject *toAdd, glm::i8vec3 const &direction);

		void		generateAllSons();

		OctreeNode		*_father;
		OctreeNode		*_sons[2][2][2];

		AABoundingBox	_node;

		uint32_t		_elementsNbr;
		CullableObject	*_elements[MAX_ELEMENT_PER_NODE];
	};
}