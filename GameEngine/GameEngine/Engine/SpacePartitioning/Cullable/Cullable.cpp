#include <SpacePartitioning/Cullable/Cullable.hh>

namespace AGE
{
	Cullable::Cullable() :
		PreparableObject(),
		type(UNKNOWN_CULLABLE),
		currentNode(NULL)
	{
	}

	Cullable::~Cullable()
	{
	}

}