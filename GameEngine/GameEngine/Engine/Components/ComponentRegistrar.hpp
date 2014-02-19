#pragma once

#include <map>
#include <functional>
#include <cereal/cereal.hpp>
#include <Components/Component.hh>

namespace Component
{
	struct Base;
};

class ComponentRegistrar
{
public:
	ComponentRegistrar();
	virtual ~ComponentRegistrar();

	template <class T>
	ComponentRegistrar &rct()
	{
		auto key = typeid(T).hash_code();
		auto it = _collection.find(key);
		if (it != std::end(_collection))
			return *this;
		_collection.insert(std::make_pair(key, new T()));
		_typeId.insert(std::make_pair(key, T::getTypeId()));
		return *this;
	}

	template <class Archive>
	Component::Base *createFromType(unsigned int type, Archive &ar, Entity e, unsigned int &typeId)
	{
		auto &it = _collection.find(type);
		auto &typeIt = _typeId.find(type);
		assert(it != std::end(_collection) || typeIt != std::end(_typeId) && "Component has not been registered");
		auto res = it->second->unserialize(ar, e);
		typeId = typeIt->second;
		return res;
	}

private:
	std::map<std::size_t, Component::Base*> _collection;
	std::map<std::size_t, unsigned int> _typeId;
};