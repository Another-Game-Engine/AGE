#include <Components/ComponentRegistrar.hpp>

namespace AGE
{
	ComponentRegistrar::ComponentRegistrar()
	{}

	ComponentRegistrar::~ComponentRegistrar()
	{
		_creationFunctions.clear();
		_typeIds.clear();
	}
}