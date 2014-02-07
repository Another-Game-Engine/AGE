#include <Components/ComponentRegistrar.hpp>

ComponentRegistrar::ComponentRegistrar()
{}

ComponentRegistrar::~ComponentRegistrar()
{
	for (auto &e : _collection)
		delete e.second;
	_collection.clear();
	_typeId.clear();
}
