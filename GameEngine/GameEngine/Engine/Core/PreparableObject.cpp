#include <Core/PreparableObject.hh>

namespace AGE
{
	PreparableObject::PreparableObject()
		: active(true),
		hasMoved(true),
		position(0.f),
		scale(1.0f)
	{

	}

	PreparableObject::~PreparableObject()
	{

	}

}