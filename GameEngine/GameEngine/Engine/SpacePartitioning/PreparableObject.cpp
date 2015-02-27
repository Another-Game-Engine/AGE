#include <SpacePartitioning/PreparableObject.hh>

namespace AGE
{
	PreparableObject::PreparableObject() :
		hasMoved(false),
		//position(0.f),
		//forward(0.f),
		//scale(1.0f)
		transformation(1)
	{

	}

	PreparableObject::~PreparableObject()
	{

	}
}