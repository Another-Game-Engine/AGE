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

void MaterialManager::compile(OpenGLTools::Framebuffer &fbo)
{
	_shaderIds.clear();
	_shaders.clear();
	_tmpList.clear();

	for (auto &e : _materials)
	{
		auto &l = e.second->getShaders();
		for (unsigned int i = 0; i < l.size(); ++i)
		{
			SmartPointer<ShaderHolder> shader = getShaderHolder(l[i]);
			for (int j = (int)(i) - 1; j >= 0; --j)
				shader->preShaders.insert(getShaderHolder(l[j])->id);
			if (i == l.size() - 1)
			{
				shader->last = true;
			}
			else
			{
				shader->last = false;
				fbo.addLayer(shader->id);
			}
			shader->materials.insert(std::make_pair(e.first, e.second));
		}
	}
	for (auto &e : _tmpList)
	{
		e.second->weight = e.second->preShaders.size();
		_shaders.insert(std::make_pair(e.second->weight, e.second));
	}
}

SmartPointer<MaterialManager::ShaderHolder> MaterialManager::getShaderHolder(const std::string &name)
{
	SmartPointer<ShaderHolder> shader;
	auto &currentIdIt = _shaderIds.find(name);
	if (currentIdIt == std::end(_shaderIds))
	{
		unsigned int id = _shaderIds.size();
		_shaderIds.insert(std::make_pair(name, id));
		shader = new ShaderHolder;
		shader->id = id;
		shader->weight = 1;
		shader->name = name;
		_tmpList.insert(std::make_pair(id, shader));
	}
	else
	{
		shader = _tmpList[currentIdIt->second];
	}
	return shader;
}
