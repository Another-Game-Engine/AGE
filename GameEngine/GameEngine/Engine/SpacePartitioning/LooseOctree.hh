#pragma once

#include <Core/CullableObjects.hh>
#include <SpacePartitioning/LooseOctreeNode.hh>
#include <Utils/MemoryPool.hpp>

namespace AGE
{
	class LooseOctree
	{
	public:
		LooseOctree(MemoryPool<Drawable> &drawables, MemoryPool<PointLight> &pointLights);
		~LooseOctree();

		void addElement(CullableShape<AABoundingBox> *toAdd);
		void removeElement(CullableShape<AABoundingBox> *toRm);
		void moveElement(CullableShape<AABoundingBox> *toMv);
		void getElementsCollide(CullableShape<Frustum> *toTest, AGE::Vector<Cullable *> &toFill);
		void cleanOctree();

		MemoryPool<LooseOctreeNode> &getNodePool();
		MemoryPool<LooseOctreeNode> const &getNodePool() const;

		MemoryPool<SOctreeElement> &getElementPool();
		MemoryPool<SOctreeElement> const &getElementPool() const;

		Cullable *getElementFromPool(PrepareKey const &key) const;

	private:
		LooseOctree();

		uint32_t	_root;

		MemoryPool<LooseOctreeNode> _nodesPool;
		MemoryPool<SOctreeElement> _elementsPool;

		MemoryPool<Drawable> &_drawablesPool;
		MemoryPool<PointLight> &_pointLightsPool;
	};
}