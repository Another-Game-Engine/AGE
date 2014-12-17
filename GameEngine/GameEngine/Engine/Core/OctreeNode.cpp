
#include <Core/OctreeNode.hh>
namespace AGE
{
	OctreeNode::OctreeNode() :
		_node(glm::vec3(-1), glm::vec3(1))
	{
		_father = NULL;
		for (uint32_t i = 0; i < 8; ++i)
			_sons[i] = NULL;
		_uniqueSubElements = 0;
		_elementsNbr = 0;
		for (size_t i = 0; i < MAX_ELEMENT_PER_NODE; ++i)
			_elements[i] = NULL;
	}

	OctreeNode::~OctreeNode()
	{

	}

	OctreeNode	*OctreeNode::addElement(CullableObject *toAdd)
	{
		glm::i8vec3	direction;
		ECollision collisionState;

		// check the collision state depending on the geometry shape
		switch (toAdd->type)
		{
		case CULLABLE_BOUNDING_BOX:
			collisionState = _node.checkCollision(((CullableBoundingBox*)toAdd)->currentAABB, direction);
			break;
		default:
			assert(!"This cullable type is not handled yet.");
			break;
		}
		// if the object is somehow in the node
		if (collisionState != OUTSIDE)
		{
			// the returned value
			OctreeNode		*newRoot = this;

			// if it is not entierly inside the node and that the node is the root, we extend the tree
			if (collisionState == INTERSECT && _father == NULL)
			{
				newRoot = extendNode(toAdd, direction);
			}
			else if (isLeaf()) // if the node is a leaf, then we add the object to the node
			{
				_elements[_elementsNbr] = toAdd;
				++_elementsNbr;
				// we split the node if there is to much objects in it
				if (_elementsNbr == MAX_ELEMENT_PER_NODE)
					splitNode();
			}
			else // if the node is not a leaf, we add the object to all it's sons
			{
				++_uniqueSubElements;
				for (uint32_t i = 0; i < 8; ++i)
				{
					if (_sons[i])
						_sons[i]->addElement(toAdd);
				}
			}
			return (newRoot);
		}
		else if (_father == NULL)
		{
			// if it's outside and there is no father, we extend the tree
			return (extendNode(toAdd, direction));
		}
		// just returns null if the the object is outside and the node is not the root (only extend by the root)
		return (NULL);
	}

	OctreeNode	*OctreeNode::removeElement(CullableObject *toRm, bool useCurrentPos)
	{
		bool collisionState;

		// check the collision state depending on the geometry shape
		switch (toRm->type)
		{
		case CULLABLE_BOUNDING_BOX:
			collisionState = _node.checkCollision(useCurrentPos ?
												((CullableBoundingBox*)toRm)->currentAABB :
												((CullableBoundingBox*)toRm)->previousAABB);
			break;
		default:
			assert(!"This cullable type is not handled yet.");
			break;
		}
		// if the cullable collide with the current node
		if (collisionState)
		{
			//if the node is a leaf, we just remove the element from the array
			if (isLeaf())
			{
				for (uint32_t i = 0; i < _elementsNbr; ++i)
				{
					if (_elements[i] == toRm)
					{
						_elements[i] = _elements[_elementsNbr - 1];
						_elements[_elementsNbr - 1] = NULL;
						--_elementsNbr;
						break;
					}
				}
			}
			else // if the node is not a leaf
			{
				// we decrement the number of elements in the subnodes
				--_uniqueSubElements;
				// if removeSons is true, we merge the sons with this node
				bool	removeSons = _uniqueSubElements < MAX_ELEMENT_PER_NODE;

				// to merge the sons, all the sons must be leafs
				for (uint32_t i = 0; i < 8; ++i)
				{
					_sons[i]->removeElement(toRm);
					if (_sons[i]->isLeaf() == false)
						removeSons = false;
				}
				// if we can remove the sons
				if (removeSons)
				{
					// for each son
					for (uint32_t i = 0; i < 8; ++i)
					{
						//  for each element in this son
						for (uint32_t e = 0; e < _sons[i]->_elementsNbr; ++e)
						{
							// if the element was not already added to the current node
							if (_sons[i]->_elements[e]->hasBeenFound == false)
							{
								// we add it
								_elements[_elementsNbr++] = _sons[i]->_elements[e];
								_sons[i]->_elements[e]->hasBeenFound = true;
							}
						}
						// then we remove the son
						delete _sons[i];
						_sons[i] = NULL;
					}
					// we reset all the elements in the node for the next search operation
					for (uint32_t i = 0; i < _elementsNbr; ++i)
						_elements[i]->hasBeenFound = false;
				}
			}
		}
		return (this);
	}

	OctreeNode	*OctreeNode::moveElement(CullableObject *toMove)
	{
		OctreeNode *newRoot = removeElement(toMove, false);
		return (newRoot->addElement(toMove));
	}

	void		OctreeNode::getElementsCollide(CullableObject *toTest, AGE::Vector<CullableObject*> &toFill) const
	{
		bool collisionState;

		switch (toTest->type)
		{
		case CULLABLE_FRUSTUM:
			collisionState = ((CullableFrustum*)toTest)->currentFrustum.checkCollision(_node);
			break;
		default:
			assert(!"This cullable type is not handled yet.");
			break;
		}
		if (collisionState)
		{
			if (isLeaf())
			{
				for (uint32_t i = 0; i < _elementsNbr; ++i)
				{
					if (_elements[i]->hasBeenFound == false &&
						toTest->checkCollision(_elements[i]))
					{
						toFill.push_back(_elements[i]);
						_elements[i]->hasBeenFound = true;
					}
				}
			}
			else
			{
				for (uint32_t i = 0; i < 8; ++i)
				{
					if (_sons[i])
						_sons[i]->getElementsCollide(toTest, toFill);
				}
			}
		}
	}

	AABoundingBox const &OctreeNode::getNodeBoundingBox() const
	{
		return (_node);
	}

	bool		OctreeNode::isLeaf() const
	{
		// If a node is not a leaf, all his sons are created
		return (_sons[0] == NULL);
	}

	void		OctreeNode::splitNode()
	{
		generateAllSons();
		for (int i = 0; i < MAX_ELEMENT_PER_NODE; ++i)
		{
			addElement(_elements[i]);
			_elements[i] = NULL;
		}
		_elementsNbr = 0;
	}

	OctreeNode * OctreeNode::extendNode(CullableObject *toAdd, glm::i8vec3 const &direction)
	{
		OctreeNode	*newRoot = new OctreeNode;
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
		newRoot->generateAllSons();
		return (newRoot->addElement(toAdd));
	}

	void		OctreeNode::generateAllSons()
	{
		for (uint32_t i = 0; i < 8; ++i)
		{
			glm::u8vec3	currentSon((i & 4) ? 1 : 0, (i & 2) ? 1 : 0, i & 1);
			if (_sons[i] == NULL)
			{
				OctreeNode	*newSon = new OctreeNode;
				glm::vec3	nodeHalfSize = (_node.maxPoint - _node.minPoint) / 2.0f;

				newSon->_father = this;
				newSon->_node.minPoint = _node.minPoint + glm::vec3(currentSon) * nodeHalfSize;
				newSon->_node.maxPoint = _node.maxPoint - glm::vec3(currentSon.x == 0 ? 1 : 0,
																	currentSon.y == 0 ? 1 : 0,
																	currentSon.z == 0 ? 1 : 0) * nodeHalfSize;

				_sons[i] = newSon;
			}
		}
	}

}