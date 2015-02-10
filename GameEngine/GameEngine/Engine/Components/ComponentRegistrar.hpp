#pragma once

#include <map>
#include <functional>
#include <cereal/cereal.hpp>

namespace AGE
{
	class AScene;
	struct ComponentBase;

	class ComponentRegistrar
	{
	private:
		ComponentRegistrar();

	public:
		~ComponentRegistrar();
		static ComponentRegistrar &getInstance()
		{
			static ComponentRegistrar instance;
			return instance;
		}

		template <class T>
		void registerComponentType()
		{
			// @ECS TODO

			//std::size_t key = typeid(T).hash_code();
			//auto it = _collection.find(key);
			//if (it != std::end(_collection))
			//	return;
			//_collection.insert(std::make_pair(key, [](){auto r = new T(); return r; }));
			//_typeId.insert(std::make_pair(key, T::getTypeId()));
			return;
		}

		template <class Archive>
		ComponentBase *createComponentFromType(std::size_t type, Archive &ar, std::size_t &typeId, AScene *scene)
		{
			// @ECS TODO

			//auto &it = _collection.find(type);
			//auto &typeIt = _typeId.find(type);
			//assert((it != std::end(_collection) || typeIt != std::end(_typeId)) && "Component has not been registered");
			//auto res = (it->second)();
			//res->_unserialize(ar, scene);
			//typeId = typeIt->second;
			//return res;

			return nullptr;
		}

		std::map<std::size_t, std::function<ComponentBase*()>> _collection;
		std::map<std::size_t, std::size_t> _typeId;
	};
}

#define REGISTER_COMPONENT_TYPE(T)(ComponentRegistrar::getInstance().registerComponentType<T>());