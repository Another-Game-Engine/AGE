
#ifndef		RESOURCEMANAGER_HH_
#define		RESOURCEMANAGER_HH_

#include "AResource.hh"
#include "Utils/SmartPointer.hh"
#include <Utils/Dependency.hpp>

#include <map>
#include <string>

class Engine;

namespace	Resources
{

class	ResourceManager : public Dependency
{
private:
	std::map<std::string, SmartPointer<AResource> >		_resources;
	Engine &_engine;
public:
	ResourceManager(Engine *engine);
	virtual ~ResourceManager();

	bool					addResource(std::string const &name,
										SmartPointer<AResource> resource,
										std::string const &param);
	bool					removeResource(std::string const &name);
	SmartPointer<AResource>	getResource(std::string const &name);
	void uninit();

};

}

#endif