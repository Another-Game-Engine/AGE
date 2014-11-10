
#include <Core/LooseOctreeNode.hh>

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
		_father = NULL;
		for (uint32_t i = 0; i < 8; ++i)
			_sons[i] = NULL;
		_uniqueSubElements = 0;
		_elements.reserve(32);
		computeLooseNode();
	}

	LooseOctreeNode::~LooseOctreeNode()
	{
		for (uint32_t i = 0; i < 8; ++i)
			delete _sons[i];
	}

	LooseOctreeNode	*LooseOctreeNode::addElement(CullableObject *toAdd)
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

			if (looseNodeSize.x > objectDimensions.x &&
				looseNodeSize.y > objectDimensions.y &&
				looseNodeSize.z > objectDimensions.z)
			{
				++_uniqueSubElements;
				// if the object is entirely in the loose node
				objectDimensions *= 2.0f;
				if (looseNodeSize.x > objectDimensions.x &&
					looseNodeSize.y > objectDimensions.y &&
					looseNodeSize.z > objectDimensions.z)
				{
					// if the object can fit in the lower level of the tree
					if (isLeaf())
						generateAllSons();
					for (uint32_t i = 0; i < 8; ++i)
						_sons[i]->addElement(toAdd);
				}
				else
				{
					// else we put the object in this node
					_elements.push_back(toAdd);
				}
				return (this);
			}
			else if (_father == NULL)
			{
				// we need to put the object in a higher level of the tree
				return (extendNode(toAdd, direction));
			}
			else
			{
				// should never happend (insertion is made from the root)
				return (NULL);
			}
		}
		else if (_father == NULL)
		{
			return (extendNode(toAdd, direction));
		}
		else
			return (NULL);
	}

	LooseOctreeNode	*LooseOctreeNode::removeElement(CullableObject *toRm, bool useCurrentPos)
	{
		bool collisionState;
		glm::vec3 objectDimensions;

		// check the collision state depending on the geometry shape
		switch (toRm->type)
		{
		case CULLABLE_BOUNDING_BOX:
			{
				AABoundingBox &tmp = useCurrentPos ?
					((CullableBoundingBox*)toRm)->currentAABB :
					((CullableBoundingBox*)toRm)->previousAABB;
				
				collisionState = _node.checkPointIn(tmp.center);
				objectDimensions = tmp.maxPoint - tmp.minPoint;
			}
			break;
		default:
			assert(!"This cullable type is not handled yet.");
			break;
		}
		// if the cullable is in the current node
		if (collisionState)
		{
			glm::vec3 looseNodeSize = _looseNode.maxPoint - _looseNode.minPoint;

			if (looseNodeSize.x < objectDimensions.x &&
				looseNodeSize.y < objectDimensions.y &&
				looseNodeSize.z < objectDimensions.z)
			{
				--_uniqueSubElements;
				// if the object is entirely in the loose node
				objectDimensions *= 2.0f;
				if (looseNodeSize.x < objectDimensions.x &&
					looseNodeSize.y < objectDimensions.y &&
					looseNodeSize.z < objectDimensions.z)
				{
					// the object is in a sub element
					for (uint32_t i = 0; i < 8; ++i)
						_sons[i]->removeElement(toRm, useCurrentPos);
					// remove the last level of the octree
					if (_uniqueSubElements == 0 && !isLeaf())
					{
						for (uint32_t i = 0; i < 8; ++i)
						{
							delete _sons[i];
							_sons[i] = NULL;
						}
					}
				}
				else
				{
					// remove the element from the node
					for (uint32_t i = 0; i < _elements.size(); ++i)
					{
						if (_elements[i] == toRm)
						{
							_elements[i] = _elements[_elements.size() - 1];
							_elements.pop_back();
							break;
						}
					}
					if (_father == NULL)
					{
						// if the node is the root, try to remove it
						uint32_t nbrSonUsed = 0;
						uint32_t sonUsedIdx;

						// if only one of its sons is used
						for (uint32_t i = 0; i < 8; ++i)
						{
							if (_sons[i]->_uniqueSubElements != 0)
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
							LooseOctreeNode *newRoot = _sons[sonUsedIdx];

							_sons[sonUsedIdx] = NULL;
							delete this;
							return (newRoot);
						}
					}
				}
			}
		}
		return (this);
	}

	LooseOctreeNode	*LooseOctreeNode::moveElement(CullableObject *toMove)
	{
		LooseOctreeNode *newRoot = removeElement(toMove, false);
		return (newRoot->addElement(toMove));
	}

	void		LooseOctreeNode::getElementsCollide(CullableObject *toTest, AGE::Vector<CullableObject*> &toFill) const
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
					_sons[i]->getElementsCollide(toTest, toFill);
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
		return (_sons[0] == NULL);
	}

	LooseOctreeNode *LooseOctreeNode::extendNode(CullableObject *toAdd, glm::i8vec3 const &direction)
	{
		LooseOctreeNode	*newRoot = new LooseOctreeNode;
		glm::vec3	nodeSize = _node.maxPoint - _node.minPoint;

		_father = newRoot;
		newRoot->_sons[ (direction.x == -1 ? 4 : 0) +
						(direction.y == -1 ? 2 : 0) +
						(direction.z == -1 ? 1 : 0)] = this;
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
		newRoot->generateAllSons();
		return (newRoot->addElement(toAdd));
	}

	void		LooseOctreeNode::generateAllSons()
	{
		for (uint32_t i = 0; i < 8; ++i)
		{
			glm::u8vec3	currentSon((i & 4) ? 1 : 0, (i & 2) ? 1 : 0, i & 1);
			if (_sons[i] == NULL)
			{
				LooseOctreeNode	*newSon = new LooseOctreeNode;
				glm::vec3	nodeHalfSize = (_node.maxPoint - _node.minPoint) / 2.0f;

				newSon->_father = this;
				newSon->_node.minPoint = _node.minPoint + glm::vec3(currentSon) * nodeHalfSize;
				newSon->_node.maxPoint = _node.maxPoint - glm::vec3(currentSon.x == 0 ? 1 : 0,
																	currentSon.y == 0 ? 1 : 0,
																	currentSon.z == 0 ? 1 : 0) * nodeHalfSize;
				newSon->_node.recomputeCenter();
				newSon->computeLooseNode();
				_sons[i] = newSon;
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

}