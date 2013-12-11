#include "Material.hh"

Material::Material(MaterialManager *manager, const std::string &name)
	: _manager(manager)
	, _name(name)
{}

Material::~Material()
{}

Material &Material::pushShader(const std::string &shader)
{
	_shaders.insert(shader);
	return *this;
}

const std::set<std::string> &Material::getShaders() const
{
	return _shaders;
}

void Material::addObject(Component::MeshRenderer *object)
{
	_meshs.insert(object);
}

void Material::removeObject(Component::MeshRenderer *object)
{
	_meshs.erase(object);
}
