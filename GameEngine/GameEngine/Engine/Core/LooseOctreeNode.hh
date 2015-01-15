#pragma once

#include <Utils/MemoryPool.hpp>
#include <Utils/AABoundingBox.hh>
#include <Utils/Containers/Vector.hpp>
#include <Core/CullableInterfaces.hh>

#define UNDEFINED_IDX 0xFFFFFFFF
#define LEAF_NODE_IDX 0xFFFFFFFE

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

		uint32_t addElement(CullableBoundingBox *toAdd, MemoryPool<LooseOctreeNode> &pool);
		// remove an element from a node
		void removeElementFromNode(CullableBoundingBox *toRm, MemoryPool<LooseOctreeNode> &pool);
		// move an element from in node
		uint32_t moveElementFromNode(CullableBoundingBox *toMv, MemoryPool<LooseOctreeNode> &pool);

		void		getElementsCollide(CullableObject *toTest, AGE::Vector<CullableObject *> &toFill, MemoryPool<LooseOctreeNode> &pool) const;

		AABoundingBox const &getNodeBoundingBox() const;
		bool				isLeaf() const;

		// clean octree function
		uint32_t tryChangeRoot(MemoryPool<LooseOctreeNode> &pool);

		void setIdx(uint32_t idx);
		void removeAllSons(MemoryPool<LooseOctreeNode> &pool);
		void removeNode(MemoryPool<LooseOctreeNode> &pool);

	private:
		// Utils methods
		uint32_t extendNode(CullableBoundingBox *toAdd, glm::i8vec3 const &direction, MemoryPool<LooseOctreeNode> &pool);
		void		generateSon(uint32_t toGenIdx, MemoryPool<LooseOctreeNode> &pool);
		void		computeLooseNode();
		// add an element in the node if the element is entierly contained in the current node
		void addElementRecursive(CullableBoundingBox *toAdd, MemoryPool<LooseOctreeNode> &pool);

		uint32_t	_thisIdx;

		uint32_t	_father;
		uint32_t	_sons[8];

		AABoundingBox _node;
		AABoundingBox _looseNode;

		// nbr of elements in all the subnodes
		uint32_t _uniqueSubElements;

		AGE::Vector<CullableObject *> _elements;
	};
}