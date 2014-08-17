#pragma once

#include <map>
#include <utility>
#include <Render/Material.hh>
#include <Render/Key.hh>

namespace gl
{
	class Shader;
	class Uniform;

	class MaterialManager
	{
	public:
		MaterialManager();
		~MaterialManager();

		Key<Material> addMaterial();
		MaterialManager &rmMaterial(Key<Material> &key);
		Key<Material> getMaterial(size_t index) const;
		template <typename TYPE> MaterialManager &setMaterial(Key<Material> const &key, typename TYPE::return_type const &value);
		template <typename TYPE> typename TYPE::return_type getMaterial(Key<Material> const &key);
		MaterialManager &updateShaderMaterial(Key<Material> const &key, Shader &shader);

	private:
		MaterialManager(MaterialManager const &copy) = delete;
		MaterialManager &operator=(MaterialManager const &m) = delete;

		std::map<Key<Material>, Material> _materials;
		std::pair<Key<Material>, Material *> _optimizeMaterialSearch;
		Material *getMaterial(Key<Material> const &key, std::string const &in);

	};

	template <typename TYPE>
	MaterialManager &MaterialManager::setMaterial(Key<Material> const &key, typename TYPE::return_type const &value)
	{
		Material *material;

		if ((material = getMaterial(key, "setMaterial")) == NULL)
			return (*this);
		material->set<TYPE>(value);
		return (*this);
	}

	template <typename TYPE>
	typename TYPE::return_type MaterialManager::getMaterial(Key<Material> const &key)
	{
		Material *material;

		material = getMaterial(key, "getMaterial");
		return (material->get<TYPE>());
	}
}