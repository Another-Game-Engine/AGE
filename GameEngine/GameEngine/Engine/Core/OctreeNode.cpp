
#include <Core/OctreeNode.hh>
#include <Core/CullableInterfaces.hh>

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

	OctreeNode	*OctreeNode::AddElement(CullableObject *toAdd)
	{
		return (NULL);
	}

	OctreeNode	*OctreeNode::MoveElement(CullableObject *toAdd)
	{
		return (NULL);
	}

	OctreeNode	*OctreeNode::removeElement(CullableObject *toRm)
	{
		return (NULL);
	}
}