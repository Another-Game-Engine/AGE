#include <SpacePartitioning/Cullable/Cullable.hh>
#include <SpacePartitioning/LooseOctreeNode.hh>

namespace AGE
{
	Cullable::Cullable() :
		PreparableObject(),
		type(UNKNOWN_CULLABLE),
		currentNode(UNDEFINED_IDX)
	{
	}

	Cullable::~Cullable()
	{
	}

}