
#include <Core/OctreeNode.hh>
#include <Core/CullableInterfaces.hh>

namespace AGE
{
	OctreeNode::OctreeNode()
	{
		_father = NULL;
		for (uint32_t i = 0; i < 8; ++i)
			_sons[i & 4][i & 2][i & 1] = NULL;
		_elementsNbr = 0;
		for (size_t i = 0; i < MAX_ELEMENT_PER_NODE; ++i)
			_elements[i] = NULL;
	}

	OctreeNode::~OctreeNode()
	{

	}

	OctreeNode	*OctreeNode::AddElement(CullableObject *toAdd)
	{
		glm::u8vec3	direction;
		ECollision collisionState;

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
			// if the node is a leaf, then we add the object to the node
			if (isLeaf())
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
				{
					if (_sons[i & 4][i & 2][i & 1] != NULL)
						_sons[i & 4][i & 2][i & 1]->AddElement(toAdd);
				}
			}
			return (newRoot);
		}
		else if (_father == NULL)
		{
			return (extendNode(toAdd, direction));
		}
		return (_father->AddElement(toAdd));
	}

	OctreeNode	*OctreeNode::MoveElement(CullableObject *toAdd)
	{
		return (NULL);
	}

	OctreeNode	*OctreeNode::removeElement(CullableObject *toRm)
	{
		return (NULL);
	}

	void		OctreeNode::splitNode()
	{

	}

	OctreeNode	*OctreeNode::extendNode(CullableObject *toAdd, glm::u8vec3 const &direction)
	{
		return (this);
	}

	bool		OctreeNode::isLeaf() const
	{
		return (_sons[0][0][0] == NULL &&
				_sons[0][0][1] == NULL &&
				_sons[0][1][0] == NULL &&
				_sons[0][1][1] == NULL &&
				_sons[1][0][0] == NULL &&
				_sons[1][0][1] == NULL &&
				_sons[1][1][0] == NULL &&
				_sons[1][1][1] == NULL);
	}

}