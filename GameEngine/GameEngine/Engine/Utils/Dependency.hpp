#pragma once

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

	inline DependenciesInjector *getDependencyManager() { return _dependencyManager; }
protected:
	DependenciesInjector *_dependencyManager;
	friend DependenciesInjector;
};
