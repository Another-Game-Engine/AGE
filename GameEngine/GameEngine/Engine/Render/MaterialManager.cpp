#include <Render/MaterialManager.hh>
#include <Render/Shader.hh>

namespace gl
{
	MaterialManager::MaterialManager()
	{

	}

	MaterialManager::~MaterialManager()
	{

	}

	MaterialManager &MaterialManager::rmMaterial(Key<Material> &key)
	{
		Material *material;
		if ((material = getMaterial(key, "rmMaterial()")) == NULL)
			return (*this);
		_materials.erase(key);
		key.destroy();
		return (*this);
	}

	Key<Material> MaterialManager::getMaterial(size_t target) const
	{
		if (target >= _materials.size())
			assert(0);
		auto &element = _materials.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	Key<Material> MaterialManager::addMaterial()
	{
		Key<Material> key;

		_materials[key];
		return (key);
	}

	Material *MaterialManager::getMaterial(Key<Material> const &key, std::string const &in)
	{
		if (!key)
			assert(0);
		if (_materials.size() == 0)
			assert(0);
		if (key == _optimizeMaterialSearch.first)
			return (_optimizeMaterialSearch.second);
		auto &material = _materials.find(key);
		if (material == _materials.end())
			assert(0);
		_optimizeMaterialSearch.first = key;
		_optimizeMaterialSearch.second = &material->second;
		return (&material->second);
	}

	MaterialManager &MaterialManager::setShader(Key<Material> const &key, Shader &shader)
	{
		Material *material;

		if ((material = getMaterial(key, "updateShaderMaterial")) == NULL)
			return (*this);
		shader.setMaterial(*material);
		return (*this);
	}
}