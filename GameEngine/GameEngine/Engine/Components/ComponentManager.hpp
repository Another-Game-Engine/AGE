#pragma once

#include <Utils/ObjectPool.hpp>
#include <memory>
#include "Component.hh"

namespace AGE
{
	class ComponentManager
	{
	private:
		std::vector<std::shared_ptr<ObjectPoolBase>> _pools;
	public:
		template <typename T, typename... Args>
		T *createComponent(const Entity &entity, Args &&...args)
		{
			auto id = Component<T>::getTypeId();

			if (_pools.size() <= id)
			{
				_pools.resize(id + 1);
				_pools[id] = std::make_shared<ObjectPool<T>>();
			}
			else if (_pools[id] == nullptr)
			{
				_pools[id] = std::make_shared<ObjectPool<T>>();
			}
			auto pool = _pools[id];
			auto voidPtr = pool->createVoidPtr();
			auto ptr = static_cast<T*>(voidPtr);
			ptr->_typeId = id;
			ptr->entity = entity;
			return ptr;
		}

		void deleteComponent(ComponentBase *component)
		{
			auto id = component->getType();
			assert(_pools.size() > id);
			assert(_pools[id] != nullptr);
			_pools[id]->destroy(component);
		}
	};
}