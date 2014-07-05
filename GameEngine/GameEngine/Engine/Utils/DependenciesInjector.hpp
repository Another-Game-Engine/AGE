#ifndef   __DEPENDENCIES_INJECTOR_HPP__
# define  __DEPENDENCIES_INJECTOR_HPP__

#include <cassert>
#include <Utils/Dependency.hpp>
#include <memory>
#include <vector>

class DependenciesInjector : public std::enable_shared_from_this<DependenciesInjector>
{
private:
	DependenciesInjector(DependenciesInjector const &);
	DependenciesInjector &operator=(DependenciesInjector const &);

	std::vector<IDependency*>             _instances;
	std::weak_ptr<DependenciesInjector>                   _parent;
public:
	DependenciesInjector(std::weak_ptr<DependenciesInjector> &&parent = std::weak_ptr<DependenciesInjector>())
		: std::enable_shared_from_this<DependenciesInjector>()
		, _parent(std::move(parent))
	{}

	virtual ~DependenciesInjector()
	{
		for (auto &e : _instances)
		{
			if (e != nullptr)
				delete e;
		}
		_instances.clear();
	}

	std::weak_ptr<DependenciesInjector> &&getDependenciesInjectorParent()
	{
		return std::forward<std::weak_ptr<DependenciesInjector>>(_parent);
	}

	template <typename T>
	T *getInstance()
	{
		std::uint16_t id = T::getTypeId();
		if (!hasInstance<T>())
		{
			auto p = _parent.lock();
			if (p)
				return p->getInstance<T>();
			else
				assert(false && "Engine Instance is not set !");
		}
		return dynamic_cast<T*>(_instances[id]);
	}

	template <typename T, typename TypeSelector = T, typename ...Args>
	T *setInstance(Args ...args)
	{
		std::uint16_t id = TypeSelector::getTypeId();
		if (_instances.size() <= id || _instances[id] == nullptr)
		{
			if (_instances.size() <= id)
				_instances.resize(id + 1, nullptr);
			assert(_instances[id] == nullptr); // instance already defined
			auto n = new T(args...);
			n->_dependencyManager = shared_from_this();
			_instances[id] = n;
		}
		return dynamic_cast<T*>(_instances[id]);
	}

	template <typename T>
	bool hasInstance()
	{
		std::uint16_t id = T::getTypeId();
		return _instances.size() > id && _instances[id] != nullptr;
	}
};

#endif    //__DEPENDENCIES_INJECTOR_HPP__