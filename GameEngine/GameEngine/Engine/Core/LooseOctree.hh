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

		void addElement(CullableObject *toAdd);
		void removeElement(CullableObject *toRm);
		void moveElement(CullableObject *toMv);
		void getElementsCollide(CullableObject *toTest, AGE::Vector<CullableObject *> &toFill);
		void cleanOctree();

	private:
		uint32_t	_root;
		MemoryPool<LooseOctreeNode> _pool;
	};
}