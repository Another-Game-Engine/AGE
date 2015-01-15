
#include <Core/LooseOctree.hh>

namespace AGE
{
	LooseOctree::LooseOctree() :
		_pool(32768 * 16)
	{
		_root = _pool.alloc();
		_pool.get(_root).setIdx(_root);
	}

	LooseOctree::~LooseOctree()
	{
		_pool.get(_root).removeAllSons(_pool);
		_pool.dealloc(_root);
	}

	void LooseOctree::addElement(CullableBoundingBox *toAdd)
	{
		_root = _pool.get(_root).addElement(toAdd, _pool);
	}

	void LooseOctree::removeElement(CullableBoundingBox *toRm)
	{
		_pool.get(toRm->currentNode).removeElementFromNode(toRm, _pool);
	}

	void LooseOctree::moveElement(CullableBoundingBox *toMv)
	{
		uint32_t newRoot = _pool.get(toMv->currentNode).moveElementFromNode(toMv, _pool);

		if (newRoot != UNDEFINED_IDX)
			_root = newRoot;
	}

	void LooseOctree::getElementsCollide(CullableObject *toTest, AGE::Vector<CullableObject *> &toFill)
	{
		_pool.get(_root).getElementsCollide(toTest, toFill, _pool);
	}

	void LooseOctree::cleanOctree()
	{
		uint32_t newRoot = _root;

		while (newRoot != UNDEFINED_IDX)
		{
			_root = newRoot;
			newRoot = _pool.get(_root).tryChangeRoot(_pool);
		}
		_pool.get(_root).removeEmptyLeafs(_pool);
	}
}