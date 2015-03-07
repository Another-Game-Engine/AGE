#pragma once

#include <Utils/MemoryPool.hpp>
#include <Utils/AABoundingBox.hh>
#include <Utils/Containers/Vector.hpp>

#define POOL_NODE_ARGS uint32_t thisIdx, LooseOctree &manager
#define CREATE_THIS_PTR LooseOctreeNode *thisPtr = &manager.getNodePool().get(thisIdx);
#define UPDATE_THIS_PTR thisPtr = &manager.getNodePool().get(thisIdx);

namespace AGE
{
	static const uint32_t UNDEFINED_IDX = 0xFFFFFFFF;
	static const uint32_t LEAF_NODE_IDX = 0xFFFFFFFE;

	class LooseOctree;

	class Frustum;

	template<typename type_t>
	struct CullableShape;

	struct SOctreeElement
	{
		PrepareKey object;
		uint32_t prev;
		uint32_t next;
	};

	class	LooseOctreeNode
	{
	public:
		LooseOctreeNode();
		~LooseOctreeNode();

		// --- FUNCTIONS MODIFYING THE NODE POOL

		static uint32_t addElement(POOL_NODE_ARGS, CullableShape<AABoundingBox> *toAdd);
		// remove an element from a node
		static void removeElementFromNode(POOL_NODE_ARGS, CullableShape<AABoundingBox> *toRm);
		// move an element from in node
		static uint32_t moveElementFromNode(POOL_NODE_ARGS, CullableShape<AABoundingBox> *toMv);
		// clean octree functions
		static uint32_t tryChangeRoot(POOL_NODE_ARGS);
		static void removeAllSons(POOL_NODE_ARGS);
		static void removeNode(POOL_NODE_ARGS);

		// --- CONST FUNCTIONS

		void checkOctreeIntegrity(LooseOctree &manager, uint32_t thisIdx) const;
		void getElementsCollide(LooseOctree &manager, CullableShape<Frustum> *toTest, AGE::Vector<Cullable *> &toFill) const;
		bool isLeaf() const;

	private:
		// --- FUNCTIONS MODIFYING THE NODE POOL

		// Utils methods
		static uint32_t extendNode(POOL_NODE_ARGS, CullableShape<AABoundingBox> *toAdd, glm::i8vec3 const &direction);
		static void generateSon(POOL_NODE_ARGS, uint32_t toGenIdx);
		// add an element in the node if the element is entierly contained in the current node
		static void addElementRecursive(POOL_NODE_ARGS, CullableShape<AABoundingBox> *toAdd);

		// --- CONST FUNCTIONS

		void computeLooseNode();
		// handle element list in node
		void addElementToList(LooseOctree &manager, Cullable *element);
		void removeElementFromList(LooseOctree &manager, Cullable *element);

		uint32_t	_father;
		uint32_t	_sons[8];

		AABoundingBox _node;
		AABoundingBox _looseNode;

		// nbr of elements in all the subnodes
		uint32_t _uniqueSubElements;

		// element list
		uint32_t _nbrElements;
		uint32_t _firstElement;
	};
}