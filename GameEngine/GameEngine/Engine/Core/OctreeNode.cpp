
#include <Core/OctreeNode.hh>
#include <Core/CullableInterfaces.hh>

namespace AGE
{
	OctreeNode::OctreeNode() :
		_node(glm::vec3(-1), glm::vec3(1))
	{
		_father = NULL;
		for (uint32_t i = 0; i < 8; ++i)
			_sons[(i & 4) == 4][(i & 2) == 2][i & 1] = NULL;
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
				for (uint32_t i = 0; i < 8; ++i)
					_sons[(i & 4) == 4][(i & 2) == 2][i & 1]->addElement(toAdd);
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

	OctreeNode	*OctreeNode::moveElement(CullableObject *toAdd)
	{
		return (NULL);
	}

	OctreeNode	*OctreeNode::removeElement(CullableObject *toRm)
	{
		glm::i8vec3	direction;
		ECollision collisionState;

		// check the collision state depending on the geometry shape
		switch (toRm->type)
		{
		case CULLABLE_BOUNDING_BOX:
			collisionState = _node.checkCollision(((CullableBoundingBox*)toRm)->currentAABB, direction);
			break;
		default:
			assert(!"This cullable type is not handled yet.");
			break;
		}
		if (collisionState != OUTSIDE)
		{
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
			else
			{
				bool	removeSons = true;

				for (uint32_t i = 0; i < 8; ++i)
				{
					_sons[(i & 4) == 4][(i & 2) == 2][i & 1]->removeElement(toRm);
					if (_sons[(i & 4) == 4][(i & 2) == 2][i & 1]->_elementsNbr != 0 ||
						_sons[(i & 4) == 4][(i & 2) == 2][i & 1]->isLeaf() == false)
						removeSons = false;
				}
				if (removeSons)
				{
					for (uint32_t i = 0; i < 8; ++i)
					{
						delete _sons[(i & 4) == 4][(i & 2) == 2][i & 1];
						_sons[(i & 4) == 4][(i & 2) == 2][i & 1] = NULL;
					}
				}
			}
		}
		return (this);
	}

	AABoundingBox const &OctreeNode::getNodeBoundingBox() const
	{
		return (_node);
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
		newRoot->_sons[direction.x == -1 ? 1 : 0][direction.y == -1 ? 1 : 0][direction.z == -1 ? 1 : 0] = this;
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

	bool		OctreeNode::isLeaf() const
	{
		// If a node is not a leaf, all his sons are created
		return (_sons[0][0][0] == NULL);
	}

	void		OctreeNode::generateAllSons()
	{
		for (uint32_t i = 0; i < 8; ++i)
		{
			glm::u8vec3	currentSon((i & 4) == 4, (i & 2) == 2, i & 1);
			if (_sons[currentSon.x][currentSon.y][currentSon.z] == NULL)
			{
				OctreeNode	*newSon = new OctreeNode;
				glm::vec3	nodeHalfSize = (_node.maxPoint - _node.minPoint) / 2.0f;

				newSon->_father = this;
				newSon->_node.minPoint = _node.minPoint + glm::vec3(currentSon) * nodeHalfSize;
				newSon->_node.maxPoint = _node.maxPoint - glm::vec3(currentSon.x == 0 ? 1 : 0,
																	currentSon.y == 0 ? 1 : 0,
																	currentSon.z == 0 ? 1 : 0) * nodeHalfSize;

				_sons[currentSon.x][currentSon.y][currentSon.z] = newSon;
			}
		}
	}

}