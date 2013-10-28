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
	std::map<unsigned int, SmartPointer<ShaderHolder> > tmpList;
	unsigned int shaderId = 0;

	for (auto &e : _materials)
	{
		auto &l = e.second->getShaders();
		for (unsigned int i = 0; i < l.size(); ++i)
		{
			unsigned int id = shaderId;
			SmartPointer<ShaderHolder> shader;
			auto &currentIdIt = _shaderIds.find(l[i]);
			if (currentIdIt == std::end(_shaderIds))
			{
				_shaderIds.insert(std::make_pair(l[i], shaderId));
				shader = new ShaderHolder;
				shader->id = shaderId;
				shader->weight = 1;
				shader->name = l[i];
				tmpList.insert(std::make_pair(shaderId, shader));
				++shaderId;
			}
			else
			{
				shader = tmpList[currentIdIt->second];
			}
			for (int j = (int)(i) - 1; j >= 0; --j)
				shader->preShaders.insert(l[j]);
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
	for (auto &e : tmpList)
	{
		e.second->weight = e.second->preShaders.size();
		_shaders.insert(std::make_pair(e.second->weight, e.second));
	}
}