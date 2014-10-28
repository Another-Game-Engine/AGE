#pragma once

#include <map>
#include <functional>
#include <cereal/cereal.hpp>
//#include <Components/Component.hh>

namespace Component
{
	struct Base;
}

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
		std::size_t key = typeid(T).hash_code();
		auto it = _collection.find(key);
		if (it != std::end(_collection))
			return;
		_collection.insert(std::make_pair(key, [](){auto r = new T(); return r; }));
		_typeId.insert(std::make_pair(key, T::getTypeId()));
		return;
	}

	template <class Archive>
	Component::Base *createComponentFromType(std::size_t type, Archive &ar, std::size_t &typeId)
	{
		auto &it = _collection.find(type);
		auto &typeIt = _typeId.find(type);
		assert((it != std::end(_collection) || typeIt != std::end(_typeId)) && "Component has not been registered");
		auto res = (it->second)();
		res->unserialize(ar);
		typeId = typeIt->second;
		return res;
	}

	std::map<std::size_t, std::function<Component::Base*()>> _collection;
	std::map<std::size_t, std::size_t> _typeId;
};

#define REGISTER_COMPONENT_TYPE(T)(ComponentRegistrar::getInstance().registerComponentType<T>());