
#include <Core/LooseOctree.hh>

namespace AGE
{
	LooseOctree::LooseOctree()
	{
		_root = new LooseOctreeNode();
	}

	LooseOctree::~LooseOctree()
	{
		delete _root;
	}

	void LooseOctree::addElement(CullableObject *toAdd)
	{
		_root = _root->addElement(toAdd);
	}

	void LooseOctree::removeElement(CullableObject *toRm)
	{
		_root->removeElement(toRm);
	}

	void LooseOctree::moveElement(CullableObject *toMv)
	{
		_root = _root->moveElement(toMv);
	}

	void LooseOctree::getElementsCollide(CullableObject *toTest, AGE::Vector<CullableObject *> &toFill) const
	{
		_root->getElementsCollide(toTest, toFill);
	}

	void LooseOctree::cleanOctree()
	{
		LooseOctreeNode *newRoot = _root;

		while (newRoot)
		{
			_root = newRoot;
			newRoot = _root->tryChangeRoot();
		}
		_root->removeEmptyLeafs();
	}
}