#ifndef    __DEPENDENCY_HPP__
# define   __DEPENDENCY_HPP__

#include <memory>

class DependenciesInjector;

class Dependency
{
public:
	Dependency()
	{}

	virtual ~Dependency()
	{}

	inline std::weak_ptr<DependenciesInjector> _getDpyMAnager() { return _dpyManager; }
protected:
	std::weak_ptr<DependenciesInjector> _dpyManager;
	friend DependenciesInjector;
};

#endif     //__DEPENDENCY_HPP__