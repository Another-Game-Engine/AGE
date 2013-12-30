#ifndef   __DEPENDENCIES_INJECTOR_HPP__
# define  __DEPENDENCIES_INJECTOR_HPP__

#include <map>
#include <typeinfo>
#include <cassert>

class DependenciesInjector
{
private:
	DependenciesInjector(DependenciesInjector const &);
	DependenciesInjector &operator=(DependenciesInjector const &);

	/////////////////////////////////////////////////////
	// Dependencie injector implementation
	// Dangerous use  of void*
	// to enhance !

	std::map<size_t, void*>         _instances;

public:
	DependenciesInjector()
	{}

	virtual ~DependenciesInjector()
	{
		for (auto &e : _instances)
			delete e.second;
	}

	template <typename T>
	T &getInstance()
	{
		const std::string tt = typeid(T).name();
		size_t id = typeid(T).hash_code();
		assert(_instances.find(id) != std::end(_instances) && "Engine Instance is not set !");
		return *(static_cast<T*>(_instances[id]));
	}

	template <typename T, typename TypeSelector = T, typename ...Args>
	T &setInstance(Args ...args)
	{
		size_t id = typeid(TypeSelector).hash_code();
		if (_instances.find(id) == std::end(_instances))
		{
			T *n = new T(args...);
			_instances.insert(std::make_pair(id, n));
		}
		return *(static_cast<T*>(_instances[id]));
	}

	template <typename T>
	void unsetInstance()
	{
		size_t id = typeid(TypeSelector).hash_code();
		if (_instances.find(id) == std::end(_instances))
			return;
		delete _instances[id];
		_instances.erase(id);
	}

	template <typename T>
	bool hasInstance()
	{
		size_t id = typeid(TypeSelector).hash_code();
		return _instances.find(id) != std::end(_instances);
	}
};

#endif    //__DEPENDENCIES_INJECTOR_HPP__