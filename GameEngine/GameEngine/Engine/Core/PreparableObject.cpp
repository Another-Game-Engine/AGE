#include <Core/PreparableObject.hh>

namespace AGE
{
	PreparableObject::PreparableObject()
		: hasMoved(false),
		position(0.f),
		forward(0.f),
		scale(1.0f)
	{

	}

	PreparableObject::~PreparableObject()
	{

	}

}