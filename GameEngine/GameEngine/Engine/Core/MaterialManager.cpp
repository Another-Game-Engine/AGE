#include "MaterialManager.hh"
#include "Material.hh"

MaterialManager::MaterialManager()
{}

MaterialManager::~MaterialManager()
{}

SmartPointer<Material> MaterialManager::getMaterial(const::std::string &name)
{
	auto &it = _materials.find(name);
	if (it == std::end(_materials))
		return nullptr;
	return it->second;
}

SmartPointer<Material> MaterialManager::createMaterial(const::std::string &name)
{
	auto &it = _materials.find(name);
	if (it != std::end(_materials))
		return it->second;
	SmartPointer<Material> m = new Material(this, name);
	_materials.insert(std::make_pair(name, m));
	return m;
}

