
#include <Core/LooseOctree.hh>

namespace AGE
{
	LooseOctree::LooseOctree() :
		_pool(32768)
	{
		_root = _pool.alloc();
		_pool.get(_root).setIdx(_root);
	}

	LooseOctree::~LooseOctree()
	{
		_pool.get(_root).removeAllSons(_pool);
		_pool.dealloc(_root);
	}

	void LooseOctree::addElement(CullableObject *toAdd)
	{
		_root = _pool.get(_root).addElement(toAdd, _pool);
		assert(_root != UNDEFINED_IDX);
	}

	void LooseOctree::removeElement(CullableObject *toRm)
	{
		_pool.get(_root).removeElement(toRm, _pool);
		assert(_root != UNDEFINED_IDX);
	}

	void LooseOctree::moveElement(CullableObject *toMv)
	{
		_root = _pool.get(_root).moveElement(toMv, _pool);
		assert(_root != UNDEFINED_IDX);
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