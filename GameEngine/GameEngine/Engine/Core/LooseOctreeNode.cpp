
#include <Core/LooseOctreeNode.hh>
#include <Core/CullableObjects.hh>

namespace AGE
{
	SOctreeElement::SOctreeElement(EElementType ptype, uint32_t pidx)
	{
		type = ptype;
		index = pidx;
	}


	static int nbrNodes = 0;

	LooseOctreeNode::LooseOctreeNode() :
		_node(glm::vec3(-1), glm::vec3(1))
	{
		_father = UNDEFINED_IDX;
		for (uint32_t i = 0; i < 8; ++i)
			_sons[i] = UNDEFINED_IDX;
		_uniqueSubElements = 0;
		_thisIdx = UNDEFINED_IDX;
		computeLooseNode();
//		nbrNodes += 1;
//		std::cout << nbrNodes << std::endl;
	}

	LooseOctreeNode::~LooseOctreeNode()
	{
//		nbrNodes -= 1;
//		std::cout << nbrNodes << std::endl;
	}

	uint32_t LooseOctreeNode::addElement(CullableBoundingBox *toAdd, MemoryPool<LooseOctreeNode> &pool)
	{
		glm::i8vec3	direction;
		bool collisionState;
		glm::vec3 objectDimensions;
			
		collisionState = _node.checkPointIn(toAdd->currentAABB.center);
		direction = _node.getDirection(toAdd->currentAABB.center);
		objectDimensions = toAdd->currentAABB.maxPoint - toAdd->currentAABB.minPoint;
		// if the center is in the loose node
		if (collisionState == true)
		{
			glm::vec3 nodeSize = _node.maxPoint - _node.minPoint;
			glm::vec3 halfNodeSize = nodeSize / 2.0f;

			if (glm::all(glm::greaterThan(nodeSize, objectDimensions)))
			{
				// if the object is entirely in the node
				++_uniqueSubElements;
				if (glm::all(glm::greaterThan(halfNodeSize, objectDimensions)))
				{
					// if the object can fit in the lower level of the tree
					uint32_t sonIdx = (direction.x == 1 ? 4 : 0) +
						(direction.y == 1 ? 2 : 0) +
						(direction.z == 1 ? 1 : 0);

					if (isLeaf())
						generateAllSons(pool);
					pool.get(_sons[sonIdx]).addElementRecursive(toAdd, pool);
				}
				else
				{
					// else we put the object in this node
					toAdd->currentIdx = _elements.size();
					toAdd->currentNode = _thisIdx;
					_elements.push_back(toAdd);
				}
				return (_thisIdx);
			}
		}
		if (_father == UNDEFINED_IDX)
			return (extendNode(toAdd, direction, pool));
		return (UNDEFINED_IDX);
	}

	void LooseOctreeNode::addElementRecursive(CullableBoundingBox *toAdd, MemoryPool<LooseOctreeNode> &pool)
	{
		glm::i8vec3	direction = _node.getDirection(toAdd->currentAABB.center);;
		glm::vec3 objectDimensions = toAdd->currentAABB.maxPoint - toAdd->currentAABB.minPoint;

		glm::vec3 nodeSize = _node.maxPoint - _node.minPoint;
		glm::vec3 halfNodeSize = nodeSize / 2.0f;

		++_uniqueSubElements;
		if (glm::all(glm::greaterThan(halfNodeSize, objectDimensions)))
		{
			// if the object can fit in the lower level of the tree
			uint32_t sonIdx = (direction.x == 1 ? 4 : 0) +
				(direction.y == 1 ? 2 : 0) +
				(direction.z == 1 ? 1 : 0);

			if (isLeaf())
				generateAllSons(pool);
			pool.get(_sons[sonIdx]).addElementRecursive(toAdd, pool);
		}
		else
		{
			// else we put the object in this node
			toAdd->currentIdx = _elements.size();
			toAdd->currentNode = _thisIdx;
			_elements.push_back(toAdd);
		}
	}

	void		LooseOctreeNode::getElementsCollide(CullableObject *toTest, AGE::Vector<CullableObject*> &toFill, MemoryPool<LooseOctreeNode> &pool) const
	{
		bool collisionState;

		switch (toTest->type)
		{
		case CULLABLE_FRUSTUM:
			collisionState = ((CullableFrustum*)toTest)->currentFrustum.checkCollision(_looseNode);
			break;
		default:
			assert(!"This cullable type is not handled yet.");
			break;
		}
		if (collisionState)
		{
			for (auto e : _elements)
			{
				if (toTest->checkCollision(e))
					toFill.push_back(e);
			}
			if (!isLeaf())
			{
				for (uint32_t i = 0; i < 8; ++i)
				{
					// octree mal clean
					assert(!(pool.get(_sons[i])._uniqueSubElements == 0 && !pool.get(_sons[i]).isLeaf()));
					if (pool.get(_sons[i])._uniqueSubElements != 0)
						pool.get(_sons[i]).getElementsCollide(toTest, toFill, pool);
				}
			}
		}
	}

	AABoundingBox const &LooseOctreeNode::getNodeBoundingBox() const
	{
		return (_node);
	}

	bool		LooseOctreeNode::isLeaf() const
	{
		// If a node is not a leaf, all his sons are created
		return (_sons[0] == UNDEFINED_IDX);
	}

	uint32_t LooseOctreeNode::tryChangeRoot(MemoryPool<LooseOctreeNode> &pool)
	{
		// if the node is the root, has no elements and has sons, we try to change it
		if (_father == UNDEFINED_IDX && !isLeaf() && _elements.size() == 0)
		{
			// if the node is the root, try to remove it
			uint32_t nbrSonUsed = 0;
			uint32_t sonUsedIdx;

			// if only one of its sons is used
			for (uint32_t i = 0; i < 8; ++i)
			{
				if (pool.get(_sons[i])._uniqueSubElements != 0)
				{
					++nbrSonUsed;
					sonUsedIdx = i;
					if (nbrSonUsed > 1)
						break;
				}
			}
			if (nbrSonUsed == 1)
			{
				// replace the root by this son
				uint32_t newRoot = _sons[sonUsedIdx];

				_sons[sonUsedIdx] = UNDEFINED_IDX;
				removeAllSons(pool);
				pool.dealloc(_thisIdx);
				pool.get(newRoot)._father = UNDEFINED_IDX;
				return (newRoot);
			}
		}
		return (UNDEFINED_IDX);
	}

	uint32_t LooseOctreeNode::extendNode(CullableBoundingBox *toAdd, glm::i8vec3 const &direction,
												MemoryPool<LooseOctreeNode> &pool)
	{
		uint32_t newRootIdx = pool.alloc();
		LooseOctreeNode	*newRoot = &pool.get(newRootIdx);
		glm::vec3	nodeSize = _node.maxPoint - _node.minPoint;

		_father = newRootIdx;
		newRoot->setIdx(newRootIdx);
		newRoot->_sons[ (direction.x == -1 ? 4 : 0) +
						(direction.y == -1 ? 2 : 0) +
						(direction.z == -1 ? 1 : 0)] = _thisIdx;
		newRoot->_uniqueSubElements = _uniqueSubElements;
		if (direction.x == -1)
		{
			newRoot->_node.minPoint.x = _node.minPoint.x - nodeSize.x;
			newRoot->_node.maxPoint.x = _node.maxPoint.x;
		}
		else
		{
			newRoot->_node.minPoint.x = _node.minPoint.x;
			newRoot->_node.maxPoint.x = _node.maxPoint.x + nodeSize.x;
		}
		if (direction.y == -1)
		{
			newRoot->_node.minPoint.y = _node.minPoint.y - nodeSize.y;
			newRoot->_node.maxPoint.y = _node.maxPoint.y;
		}
		else
		{
			newRoot->_node.minPoint.y = _node.minPoint.y;
			newRoot->_node.maxPoint.y = _node.maxPoint.y + nodeSize.y;
		}
		if (direction.z == -1)
		{
			newRoot->_node.minPoint.z = _node.minPoint.z - nodeSize.z;
			newRoot->_node.maxPoint.z = _node.maxPoint.z;
		}
		else
		{
			newRoot->_node.minPoint.z = _node.minPoint.z;
			newRoot->_node.maxPoint.z = _node.maxPoint.z + nodeSize.z;
		}
		newRoot->_node.recomputeCenter();
		newRoot->computeLooseNode();
		newRoot->generateAllSons(pool);
		return (newRoot->addElement(toAdd, pool));
	}

	void		LooseOctreeNode::generateAllSons(MemoryPool<LooseOctreeNode> &pool)
	{
		for (uint32_t i = 0; i < 8; ++i)
		{
			glm::u8vec3	currentSon((i & 4) ? 1 : 0, (i & 2) ? 1 : 0, i & 1);
			if (_sons[i] == UNDEFINED_IDX)
			{
				uint32_t newSonIdx = pool.alloc();
				LooseOctreeNode	*newSon = &pool.get(newSonIdx);
				glm::vec3	nodeHalfSize = (_node.maxPoint - _node.minPoint) / 2.0f;

				newSon->setIdx(newSonIdx);
				assert(newSonIdx != UNDEFINED_IDX);
				newSon->_father = _thisIdx;
				newSon->_node.minPoint = _node.minPoint + glm::vec3(currentSon) * nodeHalfSize;
				newSon->_node.maxPoint = _node.maxPoint - glm::vec3(currentSon.x == 0 ? 1 : 0,
																	currentSon.y == 0 ? 1 : 0,
																	currentSon.z == 0 ? 1 : 0) * nodeHalfSize;
				newSon->_node.recomputeCenter();
				newSon->computeLooseNode();
				_sons[i] = newSonIdx;
			}
		}
	}

	void		LooseOctreeNode::computeLooseNode()
	{
		glm::vec3 dimensions = _node.maxPoint - _node.minPoint;
		_looseNode.minPoint = _node.minPoint - (dimensions / 2.0f);
		_looseNode.maxPoint = _node.maxPoint + (dimensions / 2.0f);
		_looseNode.recomputeCenter();
	}

	void LooseOctreeNode::removeElementFromNode(CullableBoundingBox *toRm, MemoryPool<LooseOctreeNode> &pool)
	{
		// remove the element from the node
		assert(toRm->currentIdx < _elements.size());
		_elements[toRm->currentIdx] = _elements[_elements.size() - 1];
		_elements[toRm->currentIdx]->currentIdx = toRm->currentIdx;
		_elements.pop_back();
		for (uint32_t curNode = _thisIdx; curNode != UNDEFINED_IDX; curNode = pool.get(curNode)._father)
		{
			--pool.get(curNode)._uniqueSubElements;
		}
		toRm->currentNode = UNDEFINED_IDX;
		toRm->currentIdx = UNDEFINED_IDX;
	}

	uint32_t LooseOctreeNode::moveElementFromNode(CullableBoundingBox *toMv, MemoryPool<LooseOctreeNode> &pool)
	{
		glm::vec3 nodeSize = _node.maxPoint - _node.minPoint;
		glm::vec3 halfNodeSize = nodeSize / 2.0f;

		bool currentCollisionState;
		glm::vec3 currentObjectDimensions;

		currentCollisionState = _node.checkPointIn(toMv->currentAABB.center);
		currentObjectDimensions = toMv->currentAABB.maxPoint - toMv->currentAABB.minPoint;

		if (currentCollisionState &&
			glm::all(glm::greaterThan(nodeSize, currentObjectDimensions)))
		{
			if (glm::all(glm::greaterThan(halfNodeSize, currentObjectDimensions)))
			{
				glm::i8vec3	direction = _node.getDirection(toMv->currentAABB.center);
				uint32_t sonIdx = (direction.x == 1 ? 4 : 0) +
								(direction.y == 1 ? 2 : 0) +
								(direction.z == 1 ? 1 : 0);
				// remove element from node
				assert(toMv->currentIdx < _elements.size());
				_elements[toMv->currentIdx] = _elements[_elements.size() - 1];
				_elements[toMv->currentIdx]->currentIdx = toMv->currentIdx;
				_elements.pop_back();
				if (isLeaf())
					generateAllSons(pool);
				pool.get(_sons[sonIdx]).addElementRecursive(toMv, pool);
			}
			// The object is still contained in the same node
			return (UNDEFINED_IDX);
		}
		// The object has changed node
		--_uniqueSubElements;

		LooseOctreeNode *prevNode = this;
		uint32_t currentNodeIdx = _father;
		LooseOctreeNode *currentNode;

		// remove the element from the node
		assert(toMv->currentIdx < _elements.size());
		_elements[toMv->currentIdx] = _elements[_elements.size() - 1];
		_elements[toMv->currentIdx]->currentIdx = toMv->currentIdx;
		_elements.pop_back();

		while (currentNodeIdx != UNDEFINED_IDX)
		{
			currentNode = &pool.get(currentNodeIdx);

			glm::vec3 nodeSize = currentNode->_node.maxPoint - currentNode->_node.minPoint;

			currentCollisionState = currentNode->_node.checkPointIn(toMv->currentAABB.center);
			currentObjectDimensions = toMv->currentAABB.maxPoint - toMv->currentAABB.minPoint;

			--currentNode->_uniqueSubElements;

			if (currentCollisionState &&
				glm::all(glm::greaterThan(nodeSize, currentObjectDimensions)))
			{
				currentNode->addElementRecursive(toMv, pool);
				return (UNDEFINED_IDX);
			}

			prevNode = currentNode;
			currentNodeIdx = currentNode->_father;
		}
		return (prevNode->addElement(toMv, pool));
	}

	void LooseOctreeNode::setIdx(uint32_t idx)
	{
		_thisIdx = idx;
	}

	void LooseOctreeNode::removeEmptyLeafs(MemoryPool<LooseOctreeNode> &pool)
	{
		if (!isLeaf())
		{
			if (_uniqueSubElements == 0)
			{
				for (int i = 0; i < 8; ++i)
				{
					pool.get(_sons[i]).removeAllSons(pool);
					pool.dealloc(_sons[i]);
					_sons[i] = UNDEFINED_IDX;
				}
			}
			else
			{
				for (int i = 0; i < 8; ++i)
					pool.get(_sons[i]).removeEmptyLeafs(pool);
			}
		}
	}

	void LooseOctreeNode::removeAllSons(MemoryPool<LooseOctreeNode> &pool)
	{
		for (uint32_t i = 0; i < 8; ++i)
		{
			if (_sons[i] != UNDEFINED_IDX)
			{
				pool.get(_sons[i]).removeAllSons(pool);
				pool.dealloc(_sons[i]);
			}
		}
	}

}