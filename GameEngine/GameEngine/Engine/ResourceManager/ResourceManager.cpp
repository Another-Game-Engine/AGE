
#include "ResourceManager.hh"

namespace	Resources
{

ResourceManager::ResourceManager(Engine *engine) :
_engine(*engine)
{

}

ResourceManager::~ResourceManager()
{
}

bool					ResourceManager::addResource(std::string const &name,
													SmartPointer<AResource> resource,
													std::string const &param)
{
	std::map<std::string, SmartPointer<AResource> >::iterator	it;

	if ((it = _resources.find(name)) != _resources.end())
		return (false);
	resource->setEngine(_engine);
	_resources[name] = resource;
	assert(resource->load(param) == true && "Resource does not load correctly.");
	return (true);
}

bool					ResourceManager::removeResource(std::string const &name)
{
	std::map<std::string, SmartPointer<AResource> >::iterator	it;

	if ((it = _resources.find(name)) == _resources.end())
		return (false);
	_resources.erase(it);
	return (true);
}

SmartPointer<AResource>	ResourceManager::getResource(std::string const &name)
{
	std::map<std::string, SmartPointer<AResource> >::iterator	it;
	
	if ((it = _resources.find(name)) == _resources.end())
		return (SmartPointer<AResource>(NULL));
	return (it->second);
}

void ResourceManager::uninit()
{
	_resources.clear();
}

}