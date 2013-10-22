
#ifndef		RESOURCEMANAGER_HH_
#define		RESOURCEMANAGER_HH_

#include "Utils/Singleton.hh"
#include "AResource.hh"
#include "Utils/SmartPointer.hh"

#include <map>
#include <string>

namespace	Resources
{

class	ResourceManager
{
private:
	std::map<std::string, SmartPointer<AResource> >		_resources;

public:
	ResourceManager();
	~ResourceManager();

	bool					addResource(std::string const &name,
										SmartPointer<AResource> resource,
										std::string const &param);
	bool					removeResource(std::string const &name);
	SmartPointer<AResource>	getResource(std::string const &name);

};

}

#endif