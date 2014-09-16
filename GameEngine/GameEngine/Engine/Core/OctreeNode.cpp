
#include <Core/OctreeNode.hh>

namespace AGE
{
	OctreeNode::OctreeNode()
	{
		_father = NULL;
		for (size_t i = 0; i < 8; ++i)
			_sons[i] = NULL;
		for (size_t i = 0; i < MAX_ELEMENT_PER_NODE; ++i)
			_elements[i] = NULL;
	}

	OctreeNode::~OctreeNode()
	{

	}

	OctreeNode	*OctreeNode::AddElement(Drawable *toAdd)
	{
		return (NULL);
	}

	OctreeNode	*OctreeNode::MoveElement(Drawable *toAdd)
	{
		return (NULL);
	}

	OctreeNode	*OctreeNode::removeElement(Drawable *toRm)
	{
		return (NULL);
	}
}