#ifndef    __DEPENDENCY_HPP__
# define   __DEPENDENCY_HPP__

#include <memory>
#include <cstdint>

class DependenciesInjector;

class IDependency
{
public:
	virtual ~IDependency()
	{}
protected:
	static std::uint16_t uniqueId()
	{
		static std::uint16_t id = 0;
		return id++;
	}
};

template <class T>
class Dependency : public IDependency
{
public:
	Dependency()
	{}

	virtual ~Dependency()
	{}

	static std::uint16_t getTypeId()
	{
		static std::uint16_t id = uniqueId();
		return id;
	}

	inline std::weak_ptr<DependenciesInjector> &&_getDpyMAnager() { return std::forward<std::weak_ptr<DependenciesInjector>>(_dpyManager); }
protected:
	std::weak_ptr<DependenciesInjector> _dpyManager;
	friend DependenciesInjector;
};

#endif     //__DEPENDENCY_HPP__