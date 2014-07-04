#ifndef    __DEPENDENCY_HPP__
# define   __DEPENDENCY_HPP__

#include <memory>
#include <cstdint>

class DependenciesInjector;

class IDependency
{
protected:
	static std::uint16_t uniqueId()
	{
		static std::uint16_t id = 0;
		return id++;
	}

	IDependency()
	{}

	virtual ~IDependency()
	{}
	friend DependenciesInjector;
};

template <class T>
class Dependency : public IDependency
{
protected:
	Dependency()
	{}

	virtual ~Dependency()
	{}
public:
	static std::uint16_t getTypeId()
	{
		static std::uint16_t id = uniqueId();
		return id;
	}

	inline std::weak_ptr<DependenciesInjector> &&getDependencyManager() { return std::forward<std::weak_ptr<DependenciesInjector>>(_dependencyManager); }
protected:
	std::weak_ptr<DependenciesInjector> _dependencyManager;
	friend DependenciesInjector;
};

#endif     //__DEPENDENCY_HPP__