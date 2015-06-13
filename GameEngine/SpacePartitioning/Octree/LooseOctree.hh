#pragma once

#include <SpacePartitioning/Octree/LooseOctreeNode.hh>
#include <GameEngine/Engine/Utils/MemoryPool.hpp>

#define DEBUG_OCTREE 0

namespace AGE
{
	struct PointLight;
	struct Drawable;

	class LooseOctree
	{
	public:
		LooseOctree(MemoryPool<Drawable> &drawables, MemoryPool<PointLight> &pointLights);
		~LooseOctree();

		void addElement(CullableShape<AABoundingBox> *toAdd);
		void removeElement(CullableShape<AABoundingBox> *toRm);
		void moveElement(CullableShape<AABoundingBox> *toMv);
		void getElementsCollide(CullableShape<Frustum> *toTest, AGE::Vector<Cullable *> &toFill);
		void getAllElements(AGE::Vector<Cullable *> &toFill);
		void cleanOctree();

		MemoryPool<LooseOctreeNode, uint32_t> &getNodePool();
		MemoryPool<LooseOctreeNode, uint32_t> const &getNodePool() const;

		MemoryPool<SOctreeElement, uint32_t> &getElementPool();
		MemoryPool<SOctreeElement, uint32_t> const &getElementPool() const;

		Cullable *getElementFromPool(PrepareKey const &key) const;

	private:
		LooseOctree();

		uint32_t	_root;

		MemoryPool<LooseOctreeNode, uint32_t> _nodesPool;
		MemoryPool<SOctreeElement, uint32_t> _elementsPool;

		MemoryPool<Drawable> &_drawablesPool;
		MemoryPool<PointLight> &_pointLightsPool;
	};
}