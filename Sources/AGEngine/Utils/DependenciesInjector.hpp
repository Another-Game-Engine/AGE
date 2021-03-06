#pragma once

#include <cassert>
#include <memory>
#include <mutex>

#include "Dependency.hpp"
#include "Containers/Vector.hpp"
#include "SpinLock.hpp"

class DependenciesInjector
{
private:
	mutable AGE::SpinLock _mutex;
	DependenciesInjector(DependenciesInjector const &) = delete;
	DependenciesInjector &operator=(DependenciesInjector const &) = delete;

	AGE::Vector<IDependency*>             _instances;
	DependenciesInjector                   *_parent;
public:
	DependenciesInjector(DependenciesInjector *parent = nullptr);

	virtual ~DependenciesInjector();

	inline DependenciesInjector *getDependenciesInjectorParent()
	{
		return _parent;
	}

	template <typename T>
	T *getInstance()
	{
		std::unique_lock<AGE::SpinLock> lock(_mutex);

		std::uint16_t id = T::getTypeId();
		if (!_unsafeHasInstance<T>())
		{
			auto p = _parent;
			if (p)
			{
				lock.unlock();
				return p->getInstance<T>();
			}
			else
				assert(false && "Instance is not set !");
		}
		return static_cast<T*>(_instances[id]);
	}

	template <typename T>
	void removeInstance(void)
	{
		std::unique_lock<AGE::SpinLock> lock(_mutex);
		std::uint16_t id = T::getTypeId();
		if (!_unsafeHasInstance<T>())
		{
			auto p = _parent;
			if (p)
			{
				lock.unlock();
				return p->removeInstance<T>();
			}
			else
				assert(false && "Instance is not set !");
		}
		static_cast<T*>(_instances[id])->_dependencyManager = nullptr;
		_instances[id] = nullptr;
	}

	template <typename T>
	void deleteInstance()
	{
		std::unique_lock<AGE::SpinLock> lock(_mutex);

		std::uint16_t id = T::getTypeId();
		if (!_unsafeHasInstance<T>())
		{
			auto p = _parent;
			if (p)
			{
				lock.unlock();
				return p->deleteInstance<T>();
			}
			else
				assert(false && "Instance is not set !");
		}
		delete static_cast<T*>(_instances[id]);
		_instances[id] = nullptr;
	}

	template <typename T, typename TypeSelector = T>
	T *setInstance(T *instance)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		std::uint16_t id = TypeSelector::getTypeId();
		if (_instances.size() <= id || _instances[id] == nullptr)
		{
			if (_instances.size() <= id)
				_instances.resize(id + 1, nullptr);
			assert(_instances[id] == nullptr); // instance already defined
			instance->_dependencyManager = this;
			_instances[id] = instance;
		}
		return static_cast<T*>(_instances[id]);
	}

	template <typename T, typename TypeSelector = T, typename ...Args>
	T *setInstance(Args ...args)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		std::uint16_t id = TypeSelector::getTypeId();
		if (_instances.size() <= id || _instances[id] == nullptr)
		{
			if (_instances.size() <= id)
				_instances.resize(id + 1, nullptr);
			assert(_instances[id] == nullptr); // instance already defined
			auto n = new T(args...);
			n->_dependencyManager = this;
			_instances[id] = n;
		}
		return static_cast<T*>(_instances[id]);
	}

	template <typename T>
	bool hasInstance() const
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		std::uint16_t id = T::getTypeId();
		return _instances.size() > id && _instances[id] != nullptr;
	}

	private:
		template <typename T>
		bool _unsafeHasInstance() const
		{
			std::uint16_t id = T::getTypeId();
			return _instances.size() > id && _instances[id] != nullptr;
		}
};