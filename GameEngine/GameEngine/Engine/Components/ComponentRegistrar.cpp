#include <Components/ComponentRegistrar.hpp>

ComponentRegistrar::ComponentRegistrar()
{}

ComponentRegistrar::~ComponentRegistrar()
{
	_collection.clear();
	_typeId.clear();
}
