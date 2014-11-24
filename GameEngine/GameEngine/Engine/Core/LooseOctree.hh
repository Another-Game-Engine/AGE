#pragma once

#include <Core/LooseOctreeNode.hh>

namespace AGE
{
	class LooseOctree
	{
	public:
		LooseOctree();
		~LooseOctree();

		void addElement(CullableObject *toAdd);
		void removeElement(CullableObject *toRm);
		void moveElement(CullableObject *toMv);
		void getElementsCollide(CullableObject *toTest, AGE::Vector<CullableObject *> &toFill) const;
		void cleanOctree();

	private:
		LooseOctreeNode *_root;
	};
}