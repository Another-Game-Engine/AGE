#include <Render/MaterialManager.hh>
#include <Render/Shader.hh>
#include <Render/Storage.hh>

namespace gl
{
	MaterialManager::MaterialManager()
		: _defaultMaterialCreated(false),
		_defaultMaterial(Key<Material>::createKey()),
		_defaultTexture(nullptr)
	{

	}

	MaterialManager::~MaterialManager()
	{
	}

	Key<Material> MaterialManager::getDefaultMaterial()
	{
		if (_defaultMaterialCreated == false)
		{
			_defaultMaterial = addMaterial();
			_defaultMaterialCreated = true;
		}
		return (_defaultMaterial);
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
		Key<Material> key = Key<Material>::createKey();

		auto element = _materials[key];
		element.set<gl::Texture_diffuse>(getDefaultTexture2D().getId());
		element.set<gl::Texture_bump>(getDefaultTexture2D().getId());
		element.set<gl::Texture_emissive>(getDefaultTexture2D().getId());
		element.set<gl::Texture_specular>(getDefaultTexture2D().getId());
		element.set<gl::Texture_normal>(getDefaultTexture2D().getId());
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

	MaterialManager &MaterialManager::createDefaultTexture2D(glm::u8vec4 const &color)
	{
		if (_defaultTexture == NULL)
		{
			_keyDefaultTexture = Key<Texture>::createKey();
			_defaultTexture = new Texture2D(2, 2, GL_RGBA8, false);
		}
		glm::u8vec4 c[4];
		for (size_t index = 0; index < 4; ++index)
			c[index] = color;
		_defaultTexture->upload(GL_RGBA, GL_UNSIGNED_BYTE, &c);
		return (*this);
	}

	MaterialManager &MaterialManager::createDefaultTexture2D(std::array<glm::u8vec4, 4> const &damColor)
	{
		if (_defaultTexture == NULL)
		{
			_keyDefaultTexture = Key<Texture>::createKey();	
			_defaultTexture = new Texture2D(2, 2, GL_RGBA8, false);
		}
		glm::u8vec4 c[4];
		for (size_t index = 0; index < 4; ++index)
			c[index] = damColor[index];
		_defaultTexture->upload(GL_RGBA, GL_UNSIGNED_BYTE, &c);
		return (*this);
	}

	Texture2D &MaterialManager::getDefaultTexture2D()
	{
		if (_defaultTexture == NULL)
			createDefaultTexture2D(glm::u8vec4(0x00));
		return (*_defaultTexture);
	}

	Key<Texture> MaterialManager::getKeyDefaultTexture2D()
	{
		if (_defaultTexture == NULL)
			createDefaultTexture2D(glm::u8vec4(0x00));
		return (_keyDefaultTexture);
	}
}