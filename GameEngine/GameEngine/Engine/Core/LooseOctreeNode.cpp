
#include <Core/LooseOctreeNode.hh>
#include <Core/CullableObjects.hh>

namespace AGE
{
	SOctreeElement::SOctreeElement(EElementType ptype, uint32_t pidx)
	{
		type = ptype;
		index = pidx;
	}

	LooseOctreeNode::LooseOctreeNode() :
		_node(glm::vec3(-1), glm::vec3(1))
	{
		_father = UNDEFINED_IDX;
		for (uint32_t i = 0; i < 8; ++i)
			_sons[i] = UNDEFINED_IDX;
		_uniqueSubElements = 0;
		_thisIdx = UNDEFINED_IDX;
		computeLooseNode();
	}

	LooseOctreeNode::~LooseOctreeNode()
	{
	}

	uint32_t LooseOctreeNode::addElement(CullableObject *toAdd, MemoryPool<LooseOctreeNode> &pool)
	{
		glm::i8vec3	direction;
		bool collisionState;
		glm::vec3 objectDimensions;
			
		// check the collision state depending on the geometry shape
		switch (toAdd->type)
		{
		case CULLABLE_BOUNDING_BOX:
			collisionState = _node.checkPointIn(((CullableBoundingBox*)toAdd)->currentAABB.center, direction);
			objectDimensions = ((CullableBoundingBox*)toAdd)->currentAABB.maxPoint -
							   ((CullableBoundingBox*)toAdd)->currentAABB.minPoint;
			break;
		default:
			assert(!"This cullable type is not handled yet.");
			break;
		}
		// if the center is in the loose node
		if (collisionState == true)
		{
			glm::vec3 looseNodeSize = _looseNode.maxPoint - _looseNode.minPoint;
			glm::vec3 halfLooseNodeSize = looseNodeSize / 2.0f;

			if (VEC3_BIGGER(looseNodeSize, objectDimensions))
			{
				// if the object is entirely in the loose node
				++_uniqueSubElements;
				if (VEC3_BIGGER(halfLooseNodeSize, objectDimensions))
				{
					// if the object can fit in the lower level of the tree
					if (isLeaf())
						generateAllSons(pool);
					for (uint32_t i = 0; i < 8; ++i)
					{
						if (pool.get(_sons[i]).addElement(toAdd, pool) != UNDEFINED_IDX)
							break;
					}
				}
				else
				{
					// else we put the object in this node
					_elements.push_back(toAdd);
					toAdd->currentNode = _thisIdx;
				}
				return (_thisIdx);
			}
			else if (_father == UNDEFINED_IDX)
			{
				// we need to put the object in a higher level of the tree
				return (extendNode(toAdd, direction, pool));
			}
			else
			{
				// should never happend (insertion is made from the root)
				return (UNDEFINED_IDX);
			}
		}
		else if (_father == UNDEFINED_IDX)
		{
			return (extendNode(toAdd, direction, pool));
		}
		else
			return (UNDEFINED_IDX);
	}

	void LooseOctreeNode::removeElement(CullableObject *toRm, MemoryPool<LooseOctreeNode> &pool)
	{
		pool.get(toRm->currentNode).removeElementFromNode(toRm, pool);
	}

	uint32_t LooseOctreeNode::moveElement(CullableObject *toMove, MemoryPool<LooseOctreeNode> &pool)
	{
		uint32_t newRoot = pool.get(toMove->currentNode).moveElementFromNode(toMove, pool);

		return ((newRoot == UNDEFINED_IDX) ? _thisIdx : newRoot);
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
					pool.get(_sons[i]).getElementsCollide(toTest, toFill, pool);
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
		if (_father == UNDEFINED_IDX && !isLeaf())
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

	uint32_t LooseOctreeNode::extendNode(CullableObject *toAdd, glm::i8vec3 const &direction,
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

	void LooseOctreeNode::removeElementFromNode(CullableObject *toRm, MemoryPool<LooseOctreeNode> &pool)
	{
		LooseOctreeNode *curNode = this;

		// remove the element from the node
		for (uint32_t i = 0; i < _elements.size(); ++i)
		{
			if (_elements[i] == toRm)
			{
				_elements[i] = _elements[_elements.size() - 1];
				_elements.pop_back();
				for (uint32_t curNode = _thisIdx; curNode != UNDEFINED_IDX; curNode = pool.get(curNode)._father)
				{
					--pool.get(curNode)._uniqueSubElements;
				}
				toRm->currentNode = UNDEFINED_IDX;
				break;
			}
		}

	}

	uint32_t LooseOctreeNode::moveElementFromNode(CullableObject *toMv, MemoryPool<LooseOctreeNode> &pool)
	{
		glm::vec3 looseNodeSize = _looseNode.maxPoint - _looseNode.minPoint;
		glm::vec3 halfLooseNodeSize = looseNodeSize / 2.0f;

		bool currentCollisionState;
		glm::vec3 currentObjectDimensions;

		// check the collision state depending on the geometry shape
		switch (toMv->type)
		{
		case CULLABLE_BOUNDING_BOX:
			currentCollisionState = _node.checkPointIn(((CullableBoundingBox*)toMv)->currentAABB.center);
			currentObjectDimensions = ((CullableBoundingBox*)toMv)->currentAABB.maxPoint -
				((CullableBoundingBox*)toMv)->currentAABB.minPoint;
			break;
		default:
			assert(!"This cullable type is not handled yet.");
			break;
		}
		if (currentCollisionState &&
			VEC3_BIGGER(looseNodeSize, currentObjectDimensions))
		{
			if (!VEC3_BIGGER(halfLooseNodeSize, currentObjectDimensions))
			{
				// The object is still in the same node
				return (UNDEFINED_IDX);
			}
			else
			{
				removeElementFromNode(toMv, pool);
				addElementFromNode(toMv, pool);
				return (UNDEFINED_IDX);
			}
		}
		else
		{
			// The object has changed node
			LooseOctreeNode *prevNode = this;
			uint32_t currentNodeIdx = this->_father;
			LooseOctreeNode *currentNode;
			LooseOctreeNode *newRoot;

			removeElementFromNode(toMv, pool);
			while (currentNodeIdx != UNDEFINED_IDX)
			{
				currentNode = &pool.get(currentNodeIdx);
				looseNodeSize = currentNode->_looseNode.maxPoint - currentNode->_looseNode.minPoint;
				halfLooseNodeSize = looseNodeSize / 2.0f;
				switch (toMv->type)
				{
				case CULLABLE_BOUNDING_BOX:
					currentCollisionState = _node.checkPointIn(((CullableBoundingBox*)toMv)->currentAABB.center);
					currentObjectDimensions = ((CullableBoundingBox*)toMv)->currentAABB.maxPoint -
						((CullableBoundingBox*)toMv)->currentAABB.minPoint;
					break;
				default:
					assert(!"This cullable type is not handled yet.");
					break;
				}
				if (currentCollisionState &&
					VEC3_BIGGER(looseNodeSize, currentObjectDimensions))
				{
					currentNode->addElementFromNode(toMv, pool);
					return (UNDEFINED_IDX);
				}
				prevNode = currentNode;
				currentNodeIdx = currentNode->_father;
			}
			return (prevNode->addElement(toMv, pool));
		}
	}

	bool LooseOctreeNode::addElementFromNode(CullableObject *toAdd, MemoryPool<LooseOctreeNode> &pool)
	{
		glm::vec3 looseNodeSize = _looseNode.maxPoint - _looseNode.minPoint;
		glm::vec3 halfLooseNodeSize = looseNodeSize / 2.0f;

		bool currentCollisionState;
		glm::vec3 currentObjectDimensions;

		switch (toAdd->type)
		{
		case CULLABLE_BOUNDING_BOX:
			currentCollisionState = _node.checkPointIn(((CullableBoundingBox*)toAdd)->currentAABB.center);
			currentObjectDimensions = ((CullableBoundingBox*)toAdd)->currentAABB.maxPoint -
				((CullableBoundingBox*)toAdd)->currentAABB.minPoint;
			break;
		default:
			assert(!"This cullable type is not handled yet.");
			break;
		}
		if (currentCollisionState &&
			VEC3_BIGGER(looseNodeSize, currentObjectDimensions))
		{
			++_uniqueSubElements;
			if (!VEC3_BIGGER(halfLooseNodeSize, currentObjectDimensions))
			{
				_elements.push_back(toAdd);
				return (true);
			}
			else
			{
				for (int i = 0; i < 8; ++i)
				{
					if (pool.get(_sons[i]).addElementFromNode(toAdd, pool) == true)
						return (true);
				}
			}
		}
		return (false);
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
				pool.get(_sons[i]).removeAllSons(pool);
				pool.dealloc(_sons[i]);
			}
		}
	}

}