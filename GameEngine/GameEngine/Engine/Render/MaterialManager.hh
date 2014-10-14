#pragma once

#include <map>
#include <utility>
#include <Render/Material.hh>
#include <Render/Key.hh>
#include <array>

namespace gl
{
	class Shader;
	struct Uniform;
	class Texture2D;
	class Texture;

	class MaterialManager
	{
	public:
		MaterialManager();
		~MaterialManager();

		Key<Material> getDefaultMaterial();
		Key<Material> addMaterial();
		Texture2D &getDefaultTexture2D();
		Key<Texture> getKeyDefaultTexture2D();
		MaterialManager &rmMaterial(Key<Material> &key);
		Key<Material> getMaterial(size_t index) const;
		template <typename TYPE> MaterialManager &setMaterial(Key<Material> const &key, typename TYPE::return_type const &value);
		template <typename TYPE> typename TYPE::return_type getMaterial(Key<Material> const &key);
		MaterialManager &setShader(Key<Material> const &key, Shader &shader);

	private:
		MaterialManager(MaterialManager const &copy) = delete;
		MaterialManager &operator=(MaterialManager const &m) = delete;
		MaterialManager &createDefaultTexture2D(glm::u8vec4 const &color);
		MaterialManager &createDefaultTexture2D(std::array<glm::u8vec4, 4> const &damColor);

		std::map<Key<Material>, Material> _materials;
		std::pair<Key<Material>, Material *> _optimizeMaterialSearch;
		Material *getMaterial(Key<Material> const &key/*, std::string const &in*/);
		bool _defaultMaterialCreated;
		Key<Material> _defaultMaterial;
		Key<Texture> _keyDefaultTexture;
		Texture2D *_defaultTexture;

	};

	template <typename TYPE>
	MaterialManager &MaterialManager::setMaterial(Key<Material> const &key, typename TYPE::return_type const &value)
	{
		Material *material;

		if ((material = getMaterial(key/*, "setMaterial"*/)) == NULL)
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