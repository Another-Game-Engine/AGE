#pragma once

#include <Core/LooseOctreeNode.hh>
#include <Utils/MemoryPool.hpp>

namespace AGE
{
	class LooseOctree
	{
	public:
		LooseOctree();
		~LooseOctree();

		void addElement(CullableBoundingBox *toAdd);
		void removeElement(CullableBoundingBox *toRm);
		void moveElement(CullableBoundingBox *toMv);
		void getElementsCollide(CullableFrustum *toTest, AGE::Vector<CullableObject *> &toFill) const;
		void cleanOctree();

	private:
		uint32_t	_root;
		MemoryPool<LooseOctreeNode> _pool;
	};
}