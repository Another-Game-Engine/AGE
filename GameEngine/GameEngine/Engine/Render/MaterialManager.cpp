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
		if ((material = getMaterial(key)) == NULL)
			return (*this);
		_materials[key.getId()] = Material();
		key.destroy();
		return (*this);
	}

	Key<Material> MaterialManager::getMaterial(size_t target) const
	{
		if (target >= _materials.size())
			assert(0);
		return Key<Material>::createKeyWithId(target);
	}

	Key<Material> MaterialManager::addMaterial()
	{
		Key<Material> key = Key<Material>::createKey();

		if (_materials.size() <= key.getId())
			_materials.resize(key.getId() + 1);
		auto &element = _materials[key.getId()];
		element.set<gl::Texture_diffuse>(getDefaultTexture2D().getId());
		element.set<gl::Texture_bump>(getDefaultTexture2D().getId());
		element.set<gl::Texture_emissive>(getDefaultTexture2D().getId());
		element.set<gl::Texture_specular>(getDefaultTexture2D().getId());
		element.set<gl::Texture_normal>(getDefaultTexture2D().getId());
		return (key);
	}

	Material *MaterialManager::getMaterial(Key<Material> const &key)
	{
		if (!key)
			assert(0);
		if (_materials.size() == 0)
			assert(0);
		auto &material = _materials[key.getId()];
		return (&material);
	}

	MaterialManager &MaterialManager::setShader(Key<Material> const &key, Shader &shader)
	{
		Material *material;

		if ((material = getMaterial(key)) == NULL)
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