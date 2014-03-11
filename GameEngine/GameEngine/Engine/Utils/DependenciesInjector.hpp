#ifndef   __DEPENDENCIES_INJECTOR_HPP__
# define  __DEPENDENCIES_INJECTOR_HPP__

#include <map>
#include <typeinfo>
#include <cassert>
#include <Utils/Dependency.hpp>
#include <memory>

class DependenciesInjector : public std::enable_shared_from_this<DependenciesInjector>
{
private:
	DependenciesInjector(DependenciesInjector const &);
	DependenciesInjector &operator=(DependenciesInjector const &);

	std::map<size_t, std::shared_ptr<Dependency>>         _instances;
	DependenciesInjector                                  *_parent;
public:
	DependenciesInjector(DependenciesInjector *parent = nullptr)
		: std::enable_shared_from_this<DependenciesInjector>()
		, _parent(parent)
	{}

	virtual ~DependenciesInjector()
	{
		_instances.clear();
	}

	template <typename T>
	std::shared_ptr<T> getInstance()
	{
		size_t id = typeid(T).hash_code();
		if (_instances.find(id) == std::end(_instances))
		{
			if (_parent)
				return _parent->getInstance<T>();
			else
				assert(false && "Engine Instance is not set !");
		}
		return std::dynamic_pointer_cast<T>(_instances[id]);
	}

	template <typename T, typename TypeSelector = T, typename ...Args>
	std::shared_ptr<T> setInstance(Args ...args)
	{
		size_t id = typeid(TypeSelector).hash_code();
		if (_instances.find(id) == std::end(_instances))
		{
			auto n = std::make_shared<T>(args...);
			n->_dpyManager = shared_from_this();
			_instances.insert(std::make_pair(id, n));
		}
		return std::dynamic_pointer_cast<T>(_instances[id]);
	}

	template <typename T>
	void unsetInstance()
	{
		if (_parent && _parent->hasInstance<T>())
		{
			_parent->unsetInstance();
			return;
		}
		size_t id = typeid(TypeSelector).hash_code();
		if (_instances.find(id) == std::end(_instances))
			return;
		_instances.erase(id);
	}

	template <typename T>
	bool hasInstance()
	{
		size_t id = typeid(T).hash_code();
		return _instances.find(id) != std::end(_instances);
	}
};

#endif    //__DEPENDENCIES_INJECTOR_HPP__