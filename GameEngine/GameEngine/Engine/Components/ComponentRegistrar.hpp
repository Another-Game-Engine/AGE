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
		std::size_t key = typeid(T).hash_code();
		auto it = _collection.find(key);
		if (it != std::end(_collection))
			return *this;
		_collection.insert(std::make_pair(key, [](Entity e){auto r = std::make_shared<T>(); return r; }));
		_typeId.insert(std::make_pair(key, T::getTypeId()));
		return *this;
	}

	template <class Archive>
	std::shared_ptr<Component::Base> createFromType(std::size_t type, Archive &ar, Entity e, std::size_t &typeId)
	{
		auto &it = _collection.find(type);
		auto &typeIt = _typeId.find(type);
		assert((it != std::end(_collection) || typeIt != std::end(_typeId)) && "Component has not been registered");
		auto res = (it->second)(e);
		res->unserialize(ar);
		typeId = typeIt->second;
		return res;
	}

private:
	std::map<std::size_t, std::function<std::shared_ptr<Component::Base>(Entity e)>> _collection;
	std::map<std::size_t, std::size_t> _typeId;
};