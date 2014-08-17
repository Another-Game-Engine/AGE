#include <Render/MaterialManager.hh>
#include <Render/Shader.hh>

# undef DEBUG_MESSAGE
# define DEBUG_MESSAGE(type, from, reason, return_type) \
	{	assert(0 && std::string(std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "].").c_str()); return return_type; }


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
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp-getMaterial(size_t target)", "the target is out of range", Key<Material>(KEY_DESTROY));
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
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - " + in, "key destroy", NULL);
		if (_materials.size() == 0)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - " + in, "no uniformBlock present in pool", NULL);
		if (key == _optimizeMaterialSearch.first)
			return (_optimizeMaterialSearch.second);
		auto &material = _materials.find(key);
		if (material == _materials.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - " + in, "uniformBlock not find", NULL);
		_optimizeMaterialSearch.first = key;
		_optimizeMaterialSearch.second = &material->second;
		return (&material->second);
	}

	MaterialManager &MaterialManager::updateShaderMaterial(Key<Material> const &key, Shader &shader)
	{
		Material *material;

		if ((material = getMaterial(key, "updateShaderMaterial")) == NULL)
			return (*this);
		shader.updateMaterial(*material);
		return (*this);
	}
}