#include <Components/Component.hh>

namespace AGE
{

	ComponentBase::ComponentBase()
		: _typeId(-1)
	{
	}

	ComponentBase::~ComponentBase()
	{
	}

	ComponentType ComponentBase::_typeCounter = 0;
}