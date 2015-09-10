#include "DependenciesInjector.hpp"

DependenciesInjector::DependenciesInjector(DependenciesInjector *parent)
	: _parent(std::move(parent))
{
}

DependenciesInjector::~DependenciesInjector()
{
	for (auto &e : _instances)
	{
		if (e != nullptr)
			delete e;
	}
	_instances.clear();
}