
#include <Core/LooseOctree.hh>

namespace AGE
{
	LooseOctree::LooseOctree()
	{
		_root = _pool.alloc();
	}

	LooseOctree::~LooseOctree()
	{
		LooseOctreeNode::removeAllSons(_root, _pool);
		_pool.dealloc(_root);
	}

	void LooseOctree::addElement(CullableBoundingBox *toAdd)
	{
		_root = LooseOctreeNode::addElement(_root, _pool, toAdd);
	}

	void LooseOctree::removeElement(CullableBoundingBox *toRm)
	{
		LooseOctreeNode::removeElementFromNode(toRm->currentNode, _pool, toRm);
	}

	void LooseOctree::moveElement(CullableBoundingBox *toMv)
	{
		uint32_t newRoot = LooseOctreeNode::moveElementFromNode(toMv->currentNode, _pool, toMv);

		if (newRoot != UNDEFINED_IDX)
			_root = newRoot;
	}

	void LooseOctree::getElementsCollide(CullableFrustum *toTest, AGE::Vector<CullableObject *> &toFill) const
	{
		_pool.get(_root).getElementsCollide(_pool, toTest, toFill);
	}

	void LooseOctree::cleanOctree()
	{
		uint32_t newRoot = _root;

		while (newRoot != UNDEFINED_IDX)
		{
			_root = newRoot;
			newRoot = LooseOctreeNode::tryChangeRoot(_root, _pool);
		}
	}
}