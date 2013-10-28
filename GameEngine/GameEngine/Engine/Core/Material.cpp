#include "Material.hh"

Material::Material(MaterialManager *manager)
	: _manager(manager)
{}

Material::~Material()
{}

Material &Material::pushShader(const std::string &shader)
{
	_shaders.push_back(shader);
}

const std::vector<std::string> &Material::getShaders() const
{
	return _shaders;
}