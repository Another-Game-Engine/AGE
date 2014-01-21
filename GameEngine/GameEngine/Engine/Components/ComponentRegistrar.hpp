#ifndef   __COMPONENT_REGISTRAR_HPP__
# define  __COMPONENT_REGISTRAR_HPP__

#include <map>
#include <functional>
#include <cereal/cereal.hpp>
#include <Components/Component.hh>

class ComponentRegistrar
{
public:
	ComponentRegistrar()
	{}

	virtual ~ComponentRegistrar()
	{}

	template <class T>
	ComponentRegistrar &rct()
	{
		auto key = typeid(T).hash_code();
		auto it = _collection.find(key);
		if (it != std::end(_collection))
			return *this;
		_collection.insert(std::make_pair(key, new T()));
		return *this;
	}

private:
	std::map<std::size_t, Component::Base*> _collection;
};

#endif    //__COMPONENT_REGISTRAR_HPP__