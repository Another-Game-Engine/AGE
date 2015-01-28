#pragma once

#include <Core/CullableObjects.hh>
#include <Core/LooseOctreeNode.hh>
#include <Utils/MemoryPool.hpp>

namespace AGE
{
	class LooseOctree
	{
	public:
		LooseOctree(MemoryPool<Drawable> &drawables, MemoryPool<PointLight> &pointLights);
		~LooseOctree();

		void addElement(CullableBoundingBox *toAdd);
		void removeElement(CullableBoundingBox *toRm);
		void moveElement(CullableBoundingBox *toMv);
		void getElementsCollide(CullableFrustum *toTest, AGE::Vector<CullableObject *> &toFill);
		void cleanOctree();

		MemoryPool<LooseOctreeNode> &getNodePool();
		MemoryPool<LooseOctreeNode> const &getNodePool() const;

		MemoryPool<SOctreeElement> &getElementPool();
		MemoryPool<SOctreeElement> const &getElementPool() const;

		CullableObject *getElementFromPool(PrepareKey const &key) const;

	private:
		LooseOctree();

		uint32_t	_root;

		MemoryPool<LooseOctreeNode> _nodesPool;
		MemoryPool<SOctreeElement> _elementsPool;

		MemoryPool<Drawable> &_drawablesPool;
		MemoryPool<PointLight> &_pointLightsPool;
	};
}