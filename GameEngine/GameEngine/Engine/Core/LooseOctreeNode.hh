#pragma once

#include <Utils/AABoundingBox.hh>
#include <Utils/Containers/Vector.hpp>
#include <Core/CullableInterfaces.hh>

#define VEC3_BIGGER(a, b) (a.x > b.x && a.y > b.y && a.z > b.z)

namespace AGE
{
	// Think about replacing octree elements pointer with that
	enum	EElementType
	{
		OE_POINT_LIGHT,
		OE_DRAWABLE
	};

	struct	SOctreeElement
	{
		EElementType type;
		uint32_t index;

		SOctreeElement(EElementType ptype, uint32_t pidx);
	};

	class	LooseOctreeNode
	{
	public:
		LooseOctreeNode();
		~LooseOctreeNode();

		LooseOctreeNode	*addElement(CullableObject *toAdd);
		LooseOctreeNode	*removeElement(CullableObject *toRm);
		LooseOctreeNode	*moveElement(CullableObject *toMove);

		void		getElementsCollide(CullableObject *toTest, AGE::Vector<CullableObject *> &toFill) const;

		AABoundingBox const &getNodeBoundingBox() const;
		bool				isLeaf() const;

	private:
		// Utils methods
		LooseOctreeNode *extendNode(CullableObject *toAdd, glm::i8vec3 const &direction);
		void		generateAllSons();
		void		computeLooseNode();
		LooseOctreeNode	*removeElementIfChangeNode(CullableObject *toRm, bool &hasBeenRemoved);

		LooseOctreeNode *_father;
		LooseOctreeNode *_sons[8];

		AABoundingBox _node;
		AABoundingBox _looseNode;

		// nbr of elements in all the subnodes
		uint32_t _uniqueSubElements;

		AGE::Vector<CullableObject *> _elements;
	};
}