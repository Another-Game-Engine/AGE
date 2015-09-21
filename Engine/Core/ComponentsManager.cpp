#pragma once

#include "ComponentManager.hpp"

namespace AGE
{
	void *ComponentManager::allocateComponent(ComponentType id)
	{
		assert(_pools.size() > id);
		assert(_pools[id] != nullptr);
		return _pools[id]->allocateObject();
	}

	void ComponentManager::deleteComponent(ComponentBase *component)
	{
		auto id = component->getType();
		assert(_pools.size() > id);
		assert(_pools[id] != nullptr);
		_pools[id]->destroy(component);
	}
}