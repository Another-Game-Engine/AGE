#pragma once

#include <cassert>
#include <Utils/Dependency.hpp>
#include <memory>
#include <Utils/Containers/Vector.hpp>
#include <mutex>

class DependenciesInjector : public std::enable_shared_from_this<DependenciesInjector>
{
private:
	std::mutex _mutex;
	DependenciesInjector(DependenciesInjector const &);
	DependenciesInjector &operator=(DependenciesInjector const &);

	AGE::Vector<IDependency*>             _instances;
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
		//std::unique_lock<std::mutex> lock(_mutex);
		//lock.lock();
		std::uint16_t id = T::getTypeId();
		if (!hasInstance<T>())
		{
			auto p = _parent.lock();
			if (p)
			{
				//lock.unlock();
				return p->getInstance<T>();
			}
			else
				assert(false && "Instance is not set !");
		}
		return static_cast<T*>(_instances[id]);
	}

	template <typename T, typename TypeSelector = T, typename ...Args>
	T *setInstance(Args ...args)
	{
		//std::lock_guard<std::mutex> lock(_mutex);
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
		return static_cast<T*>(_instances[id]);
	}

	template <typename T>
	bool hasInstance()
	{
		std::uint16_t id = T::getTypeId();
		return _instances.size() > id && _instances[id] != nullptr;
	}
};