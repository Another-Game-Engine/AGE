
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
		_nbrElements = 0;
		_elements = NULL;
		computeLooseNode();
	}

	LooseOctreeNode::~LooseOctreeNode()
	{
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

					if (_sons[sonIdx] == UNDEFINED_IDX || _sons[sonIdx] == LEAF_NODE_IDX)
						generateSon(sonIdx, pool);
					pool.get(_sons[sonIdx]).addElementRecursive(toAdd, pool);
				}
				else
				{
					// else we put the object in this node
					toAdd->currentNode = _thisIdx;
					addElementToList(toAdd);
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

			if (_sons[sonIdx] == UNDEFINED_IDX || _sons[sonIdx] == LEAF_NODE_IDX)
				generateSon(sonIdx, pool);
			pool.get(_sons[sonIdx]).addElementRecursive(toAdd, pool);
		}
		else
		{
			// else we put the object in this node
			toAdd->currentNode = _thisIdx;
			addElementToList(toAdd);
		}
	}

	void		LooseOctreeNode::getElementsCollide(CullableFrustum *toTest, AGE::Vector<CullableObject*> &toFill, MemoryPool<LooseOctreeNode> &pool) const
	{
		assert(_uniqueSubElements != 0);
		bool collisionState;

		collisionState = toTest->currentFrustum.checkCollision(_looseNode);
		if (collisionState)
		{
			for (CullableObject *e = _elements; e != NULL; e = e->nextElement)
			{
				if (toTest->checkCollision(e))
					toFill.push_back(e);
			}
			if (!isLeaf())
			{
				for (uint32_t i = 0; i < 8; ++i)
				{
					assert(_sons[i] != UNDEFINED_IDX);
					if (_sons[i] != LEAF_NODE_IDX)
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
		if (_father == UNDEFINED_IDX && !isLeaf() && _nbrElements == 0)
		{
			// if the node is the root, try to remove it
			uint32_t nbrSonUsed = 0;
			uint32_t sonUsedIdx;

			// if only one of its sons is used
			for (uint32_t i = 0; i < 8; ++i)
			{
				if (_sons[i] != LEAF_NODE_IDX && pool.get(_sons[i])._uniqueSubElements != 0)
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

		if (_uniqueSubElements == 0)
		{
			removeNode(pool);
		}
		else
		{
			newRoot->_sons[(direction.x == -1 ? 4 : 0) +
				(direction.y == -1 ? 2 : 0) +
				(direction.z == -1 ? 1 : 0)] = _thisIdx;
			for (int i = 0; i < 8; ++i)
			{
				if (newRoot->_sons[i] == UNDEFINED_IDX)
					newRoot->_sons[i] = LEAF_NODE_IDX;
			}
		}

		newRoot->_uniqueSubElements = _uniqueSubElements;

		return (newRoot->addElement(toAdd, pool));
	}

	void		LooseOctreeNode::generateSon(uint32_t toGenIdx, MemoryPool<LooseOctreeNode> &pool)
	{
		glm::u8vec3	currentSon((toGenIdx & 4) ? 1 : 0, (toGenIdx & 2) ? 1 : 0, toGenIdx & 1);
		uint32_t newSonIdx = pool.alloc();
		LooseOctreeNode	*newSon = &pool.get(newSonIdx);
		glm::vec3	nodeHalfSize = (_node.maxPoint - _node.minPoint) / 2.0f;

		if (_sons[toGenIdx] == UNDEFINED_IDX)
		{
			for (int i = 0; i < 8; ++i)
				_sons[i] = LEAF_NODE_IDX;
		}
		newSon->setIdx(newSonIdx);
		assert(newSonIdx != UNDEFINED_IDX);
		newSon->_father = _thisIdx;
		newSon->_node.minPoint = _node.minPoint + glm::vec3(currentSon) * nodeHalfSize;
		newSon->_node.maxPoint = _node.maxPoint - glm::vec3(currentSon.x == 0 ? 1 : 0,
			currentSon.y == 0 ? 1 : 0,
			currentSon.z == 0 ? 1 : 0) * nodeHalfSize;
		newSon->_node.recomputeCenter();
		newSon->computeLooseNode();
		_sons[toGenIdx] = newSonIdx;
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
		removeElementFromList(toRm);

		LooseOctreeNode *toClean = NULL;

		for (uint32_t curNode = _thisIdx; curNode != UNDEFINED_IDX; curNode = pool.get(curNode)._father)
		{
			LooseOctreeNode *current = &pool.get(curNode);

			--current->_uniqueSubElements;
			if (current->_uniqueSubElements == 0)
				toClean = current;
		}
		if (toClean)
			toClean->removeNode(pool);
		toRm->currentNode = UNDEFINED_IDX;
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
				removeElementFromList(toMv);

				if (_sons[sonIdx] == UNDEFINED_IDX || _sons[sonIdx] == LEAF_NODE_IDX)
					generateSon(sonIdx, pool);
				pool.get(_sons[sonIdx]).addElementRecursive(toMv, pool);
			}
			// The object is still contained in the same node
			return (UNDEFINED_IDX);
		}
		// The object has changed node
		--_uniqueSubElements;

		// remove the element from the node
		removeElementFromList(toMv);

		LooseOctreeNode *prevNode = this;
		uint32_t currentNodeIdx = _father;
		LooseOctreeNode *currentNode;
		LooseOctreeNode *toClean = (_uniqueSubElements == 0) ? this : NULL;

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
				if (toClean)
					toClean->removeNode(pool);
				currentNode->addElementRecursive(toMv, pool);
				return (UNDEFINED_IDX);
			}
			else if (currentNode->_uniqueSubElements == 0)
			{
				toClean = currentNode;
			}

			prevNode = currentNode;
			currentNodeIdx = currentNode->_father;
		}
		if (toClean)
			toClean->removeNode(pool);
		return (prevNode->addElement(toMv, pool));
	}

	void LooseOctreeNode::setIdx(uint32_t idx)
	{
		_thisIdx = idx;
	}

	void LooseOctreeNode::removeAllSons(MemoryPool<LooseOctreeNode> &pool)
	{
		for (uint32_t i = 0; i < 8; ++i)
		{
			if (_sons[i] != UNDEFINED_IDX)
			{
				if (_sons[i] != LEAF_NODE_IDX)
				{
					pool.get(_sons[i]).removeAllSons(pool);
					pool.dealloc(_sons[i]);
				}
				_sons[i] = UNDEFINED_IDX;
			}
		}
	}

	void LooseOctreeNode::removeNode(MemoryPool<LooseOctreeNode> &pool)
	{
		uint32_t nodeIdx = _thisIdx;
		uint32_t father = pool.get(nodeIdx)._father;
		uint32_t idx;

		pool.get(nodeIdx).removeAllSons(pool);
		if (father != UNDEFINED_IDX)
		{
			for (idx = 0; pool.get(father)._sons[idx] != nodeIdx; ++idx);
			pool.get(father)._sons[idx] = LEAF_NODE_IDX;
			pool.dealloc(nodeIdx);
		}
	}

	void LooseOctreeNode::addElementToList(CullableObject *toAdd)
	{
		++_nbrElements;
		toAdd->nextElement = _elements;
		if (_elements)
			_elements->prevElement = toAdd;
		toAdd->prevElement = NULL;
		_elements = toAdd;
	}

	void LooseOctreeNode::removeElementFromList(CullableObject *toRm)
	{
		--_nbrElements;
		if (toRm->prevElement)
			toRm->prevElement->nextElement = toRm->nextElement;
		else
			_elements = toRm->nextElement;
		if (toRm->nextElement)
			toRm->nextElement->prevElement = toRm->prevElement;
	}

}