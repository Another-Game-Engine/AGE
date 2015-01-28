#include <Components/ComponentRegistrar.hpp>

namespace AGE
{
	ComponentRegistrar::ComponentRegistrar()
	{}

	ComponentRegistrar::~ComponentRegistrar()
	{
		_collection.clear();
		_typeId.clear();
	}
}