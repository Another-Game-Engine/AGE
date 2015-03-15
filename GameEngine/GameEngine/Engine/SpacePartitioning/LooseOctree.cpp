#include <SpacePartitioning/LooseOctree.hh>
#include <SpacePartitioning/Cullable/CullableShape.hh>
#include <SpacePartitioning/Cullable/Object/PointLight.hh>
#include <SpacePartitioning/Cullable/Object/Mesh.hh>
#include <Utils/AABoundingBox.hh>

namespace AGE
{
	LooseOctree::LooseOctree(MemoryPool<Drawable> &drawables, MemoryPool<PointLight> &pointLights) :
		_drawablesPool(drawables),
		_pointLightsPool(pointLights)
	{
		_root = _nodesPool.alloc();
	}

	LooseOctree::~LooseOctree()
	{
		LooseOctreeNode::removeAllSons(_root, *this);
		_nodesPool.dealloc(_root);
	}

	void LooseOctree::addElement(CullableShape<AABoundingBox> *toAdd)
	{
		_root = LooseOctreeNode::addElement(_root, *this, toAdd);
#if DEBUG_OCTREE
		_nodesPool.get(_root).checkOctreeIntegrity(*this, _root);
#endif
	}

	void LooseOctree::removeElement(CullableShape<AABoundingBox> *toRm)
	{
		LooseOctreeNode::removeElementFromNode(toRm->currentNode, *this, toRm);
#if DEBUG_OCTREE
		_nodesPool.get(_root).checkOctreeIntegrity(*this, _root);
#endif
	}

	void LooseOctree::moveElement(CullableShape<AABoundingBox> *toMv)
	{
		uint32_t newRoot = LooseOctreeNode::moveElementFromNode(toMv->currentNode, *this, toMv);

		if (newRoot != UNDEFINED_IDX)
			_root = newRoot;
#if DEBUG_OCTREE
		_nodesPool.get(_root).checkOctreeIntegrity(*this, _root);
#endif
	}

	void LooseOctree::getElementsCollide(CullableShape<Frustum> *toTest, AGE::Vector<Cullable *> &toFill)
	{
		_nodesPool.get(_root).getElementsCollide(*this, toTest, toFill);
#if DEBUG_OCTREE
		_nodesPool.get(_root).checkOctreeIntegrity(*this, _root);
#endif
	}

	void LooseOctree::cleanOctree()
	{
		uint32_t newRoot = _root;

		while (newRoot != UNDEFINED_IDX)
		{
			_root = newRoot;
			newRoot = LooseOctreeNode::tryChangeRoot(_root, *this);
		}
#if DEBUG_OCTREE
		_nodesPool.get(_root).checkOctreeIntegrity(*this, _root);
#endif
	}

	MemoryPool<LooseOctreeNode, uint32_t> &LooseOctree::getNodePool()
	{
		return (_nodesPool);
	}

	MemoryPool<LooseOctreeNode, uint32_t> const &LooseOctree::getNodePool() const
	{
		return (_nodesPool);
	}

	MemoryPool<SOctreeElement, uint32_t> &LooseOctree::getElementPool()
	{
		return (_elementsPool);
	}

	MemoryPool<SOctreeElement, uint32_t> const &LooseOctree::getElementPool() const
	{
		return (_elementsPool);
	}

	Cullable *LooseOctree::getElementFromPool(PrepareKey const &key) const
	{
		switch (key.type)
		{
		case PrepareKey::Drawable:
			return (&_drawablesPool.get(key.id));
			break;
		case PrepareKey::PointLight:
			return (&_pointLightsPool.get(key.id));
			break;
		default:
			return (nullptr);
			break;
		}
	}

}