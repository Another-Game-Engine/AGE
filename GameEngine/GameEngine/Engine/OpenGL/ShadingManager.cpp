#include <OpenGL/ShadingManager.hh>
#include <OpenGL/Shader.hh>
#include <OpenGL/Texture.hh>
#include <iostream>
#include <string>

namespace gl
{
	ShadingManager::ShadingManager()
	{
	}

	ShadingManager::~ShadingManager()
	{
	}

	Key<Shader> ShadingManager::addShader(std::string const &compute)
	{
		Key<Shader> key;
		Shader shader(compute);

		if (!shader.isValid())
			DEBUG_MESSAGE("Warning", "ShadingManager-addShader(string const &compute)", "compute invalid", Key<Shader>(KEY_DESTROY))
		_shaders[key] = shader;
		return (key);
	}

	Key<Shader> ShadingManager::addShader(std::string const &vertex, std::string const &frag)
	{
		Key<Shader> key;
		Shader shader(vertex, frag);

		if (!shader.isValid())
			DEBUG_MESSAGE("Warning", "ShadingManager-addShader(string const &frag, string const &vertex)", "frag and vertex invalid", Key<Shader>(KEY_DESTROY))
		_shaders[key] = shader;
		return (key);
	}

	Key<Shader> ShadingManager::addShader(std::string const &geo, std::string const &vertex, std::string const &frag)
	{
		Key<Shader> key;
		Shader shader(vertex, frag, geo);

		if (!shader.isValid())
			DEBUG_MESSAGE("Warning", "ShadingManager-addShader(string const &frag, string const &vertex, string const &geo)", "frag, vertex or geo invalid", Key<Shader>(KEY_DESTROY))
		_shaders[key] = shader;
		return (key);
	}

	ShadingManager &ShadingManager::rmShader(Key<Shader> &key)
	{
		Shader *shader;
		if ((shader = getShader(key, "rmShader()")) == NULL)
			return (*this);
		_shaders.erase(key);
		key.destroy();
		return (*this);
	}

	Key<Shader> ShadingManager::getShader(size_t target) const
	{
		if (target >= _shaders.size())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp-getShader(size_t target)", "the target is out of range", Key<Shader>(KEY_DESTROY));
		auto &element = _shaders.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	ShadingManager &ShadingManager::useShader(Key<Shader> const &key)
	{
		Shader *shader;
		if ((shader = getShader(key, "useShader()")) == NULL)
			return (*this); 
		shader->use();
		return (*this);
	}

	Key<Uniform> ShadingManager::addShaderUniform(Key<Shader> const &key, std::string const &flag)
	{
		Shader *shader;
		if ((shader = getShader(key, "addShaderUniform()")) == NULL)
			return (Key<Uniform>(KEY_DESTROY));
		return (shader->addUniform(flag));
	}

	ShadingManager &ShadingManager::rmShaderUniform(Key<Shader> const &key, Key<Uniform> &uniform)
	{
		Shader *shader;
		if ((shader = getShader(key, "rmShaderUniform()")) == NULL)
			return (*this);
		shader->rmUniform(uniform);
		return (*this);
	}

	Key<Uniform> ShadingManager::getShaderUniform(Key<Shader> const &key, size_t target)
	{
		Shader const *shader;
		if ((shader = getShader(key, "getShaderUniform()")) == NULL)
			return (Key<Uniform>(KEY_DESTROY));
		return (shader->getUniform(target));
	}

	Key<Uniform> ShadingManager::addShaderUniform(Key<Shader> const &key, std::string const &flag, glm::mat4 const &value)
	{
		Shader *shader;
		if ((shader = getShader(key, "addShaderUniform(mat4)")) == NULL)
			return (Key<Uniform>(KEY_DESTROY));
		return (shader->addUniform(flag, value));
	}

	ShadingManager &ShadingManager::setShaderUniform(Key<Shader> const &keyShader, Key<Uniform> const &key, glm::mat4 const &mat4)
	{
		Shader *shader;
		if ((shader = getShader(keyShader, "setShaderUniform(mat4)")) == NULL)
			return (*this);
		shader->setUniform(key, mat4);
		return (*this);
	}

	ShadingManager &ShadingManager::setShaderUniform(Key<Shader> const &keyShader, Key<Uniform> const &key, glm::vec4 const &vec4)
	{
		Shader *shader;
		if ((shader = getShader(keyShader, "setShaderUniform(vec4)")) == NULL)
			return (*this);
		shader->setUniform(key, vec4);
		return (*this);
	}

	ShadingManager &ShadingManager::setShaderUniform(Key<Shader> const &keyShader, Key<Uniform> const &key, float v)
	{
		Shader *shader;
		if ((shader = getShader(keyShader, "setShaderUniform(float)")) == NULL)
			return (*this);
		shader->setUniform(key, v);
		return (*this);
	}

	ShadingManager &ShadingManager::setShaderUniform(Key<Shader> const &keyShader, Key<Uniform> const &key, glm::mat3 const &mat3)
	{
		Shader *shader;
		if ((shader = getShader(keyShader, "setShaderUniform(mat3)")) == NULL)
			return (*this);
		shader->setUniform(key, mat3);
		return (*this);
	}

	Key<Sampler> ShadingManager::addShaderSampler(Key<Shader> const &keyShader, std::string const &flag)
	{
		Shader *shader;
		if ((shader = getShader(keyShader, "addShaderSampler()")) == NULL)
			return (Key<Sampler>(KEY_DESTROY));
		return (shader->addSampler(flag));
	}

	ShadingManager &ShadingManager::rmShaderSampler(Key<Shader> const &key, Key<Sampler> &uniform)
	{
		Shader *shader;
		if ((shader = getShader(key, "rmShaderSampler()")) == NULL)
			return (*this);
		shader->rmSampler(uniform);
		return (*this);
	}

	Key<Sampler> ShadingManager::getShaderSampler(Key<Shader> const &keyShader, size_t target)
	{
		Shader const *shader;
		if ((shader = getShader(keyShader, "getShaderSampler()")) == NULL)
			return (Key<Sampler>(KEY_DESTROY));
		return (shader->getSampler(target));
	}

	ShadingManager &ShadingManager::setShaderSampler(Key<Shader> const &keyShader, Key<Sampler> const &keySampler, Key<Texture> const &keyTexture)
	{
		Shader *shader;
		if ((shader = getShader(keyShader, "setShaderSampler()")) == NULL)
			return (*this);
		Texture *texture;
		if ((texture = getTexture(keyTexture, "setShaderSampler()", GL_NONE)) == NULL)
			return (*this);
		shader->setSampler(keySampler, *texture);
		return (*this);
	}

	Key<UniformBlock> ShadingManager::addUniformBlock(size_t nbrElement, size_t *sizeElement)
	{
		Key<UniformBlock> key;

		_uniformBlock[key] = UniformBlock(nbrElement, sizeElement);
		return (key);
	}

	ShadingManager &ShadingManager::rmUniformBlock(Key<UniformBlock> &key)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - rmUniformBlock", "the key is destroy", *this)
		_uniformBlock.erase(key);
		key.destroy();
		return (*this);
	}

	Key<UniformBlock> ShadingManager::getUniformBlock(size_t target) const
	{
		if (target >= _uniformBlock.size())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp-getUniformBlock(size_t target)", "the target is out of range", Key<UniformBlock>(KEY_DESTROY));
		auto &element = _uniformBlock.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	Key<InterfaceBlock> ShadingManager::addShaderInterfaceBlock(Key<Shader> const &keyShader, std::string const &flag)
	{
		Shader *shader;
		if ((shader = getShader(keyShader, "getShaderInterfaceBlock()")) == NULL)
			return (Key<InterfaceBlock>(KEY_DESTROY));
		return (shader->addInterfaceBlock(flag));
	}

	Key<InterfaceBlock> ShadingManager::addShaderInterfaceBlock(Key<Shader> const &keyShader, std::string const &flag, Key<UniformBlock> const &keyUniformBlock)
	{
		Shader *shader;
		if ((shader = getShader(keyShader, "addShaderInterfaceBlock()")) == NULL)
			return (Key<InterfaceBlock>(KEY_DESTROY));
		UniformBlock *uniformBlock;
		if ((uniformBlock = getUniformBlock(keyUniformBlock, "addShaderInterfaceBlock()")) == NULL)
			return (Key<InterfaceBlock>(KEY_DESTROY));
		return (shader->addInterfaceBlock(flag, *uniformBlock));
	}

	ShadingManager &ShadingManager::rmShaderInterfaceBlock(Key<Shader> const &keyShader, Key<InterfaceBlock> &interfaceblock)
	{
		Shader *shader;
		if ((shader = getShader(keyShader, "rmShaderInterfaceBlock()")) == NULL)
			return (*this);
		shader->rmInterfaceBlock(interfaceblock);
		return (*this);
	}

	Key<InterfaceBlock> ShadingManager::getShaderInterfaceBlock(Key<Shader> const &keyShader, size_t target)
	{
		Shader const *shader;
		if ((shader = getShader(keyShader, "getShaderInterfaceBlock()")) == NULL)
			return (Key<InterfaceBlock>(KEY_DESTROY));
		return (shader->getInterfaceBlock(target));
	}

	ShadingManager &ShadingManager::setShaderInterfaceBlock(Key<Shader> const &keyShader, Key<InterfaceBlock> const &keyInterfaceBlock, Key<UniformBlock> const &keyUniformBlock)
	{
		Shader *shader;
		if ((shader = getShader(keyShader, "setShaderInterfaceBlock()")) == NULL)
			return (*this);
		UniformBlock *uniformBlock;
		if ((uniformBlock = getUniformBlock(keyUniformBlock, "setShaderInterfaceBlock()")) == NULL)
			return (*this);
		shader->setInterfaceBlock(keyInterfaceBlock, *uniformBlock);
		return (*this);
	}

	Key<Texture> ShadingManager::addTexture2D(GLenum internalFormat, GLsizei width, GLsizei height, bool mipmapping)
	{
		Key<Texture> key;

		_textures[key] = new Texture2D(internalFormat, width, height, mipmapping);
		return (key);
	}

	Key<Texture> ShadingManager::addTextureMultiSample(GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocation)
	{
		Key<Texture> key;

		_textures[key] = new TextureMultiSample(samples, internalFormat, width, height, fixedSampleLocation);
		return (key);
	}

	ShadingManager &ShadingManager::rmTexture(Key<Texture> &key)
	{
		Texture *texture;
		if ((texture = getTexture(key, "rmTexture()", GL_NONE)) == NULL)
			return (*this);
		delete texture;
		_textures.erase(key);
		key.destroy();
		return (*this);
	}

	Key<Texture> ShadingManager::getTexture(size_t target) const
	{
		if (target >= _textures.size())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp-getTexture(size_t target)", "the target is out of range", Key<Texture>(KEY_DESTROY));
		auto &element = _textures.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	GLenum ShadingManager::getTypeTexture(Key<Texture> const &key)
	{
		Texture const *texture;
		if ((texture = getTexture(key, "getTypeTexture()", GL_NONE)) == NULL)
			return (GL_NONE);
		return (texture->getType());
	}

	Shader *ShadingManager::getShader(Key<Shader> const &key, std::string const &in)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - " + in, "key destroy", NULL);
		if (_shaders.size() == 0)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - " + in, "no shader present in pool", NULL);
		if (key == _optimizeShaderSearch.first)
			return (_optimizeShaderSearch.second);
		auto &shader = _shaders.find(key);
		if (shader == _shaders.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - " + in, "shader not find", NULL);
		_optimizeShaderSearch.first = key;
		_optimizeShaderSearch.second = &shader->second;
		return (&shader->second);
	}

	Texture *ShadingManager::getTexture(Key<Texture> const &key, std::string const &in, GLenum type)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - " + in, "key destroy", NULL);
		if (_textures.size() == 0)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - " + in, "no texture present in pool", NULL);
		if (key == _optimizeTextureSearch.first)
			return (_optimizeTextureSearch.second);
		auto &texture = _textures.find(key);
		if (texture == _textures.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - " + in, "texture not find", NULL);
		if (texture->second->getType() != type && type != GL_NONE)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - " + in, "error cast on texture", NULL);
		_optimizeTextureSearch.first = key;
		_optimizeTextureSearch.second = texture->second;
		return (texture->second);
	}

	UniformBlock *ShadingManager::getUniformBlock(Key<UniformBlock> const &key, std::string const &in)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - " + in, "key destroy", NULL);
		if (_uniformBlock.size() == 0)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - " + in, "no uniformBlock present in pool", NULL);
		if (key == _optimizeUniformBlockSearch.first)
			return (_optimizeUniformBlockSearch.second);
		auto &uniformBlock = _uniformBlock.find(key);
		if (uniformBlock == _uniformBlock.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - " + in, "uniformBlock not find", NULL);
		_optimizeUniformBlockSearch.first = key;
		_optimizeUniformBlockSearch.second = &uniformBlock->second;
		return (&uniformBlock->second);
	}

	ShadingManager &ShadingManager::wrapTexture2D(Key<Texture> const &key, GLint param)
	{
		Texture2D const *texture;
		if ((texture = (Texture2D *)getTexture(key, "wrapTexture2D", GL_TEXTURE_2D)) == NULL)
			return (*this);
		texture->wrap(param);
		return (*this);
	}

	ShadingManager &ShadingManager::filterTexture2D(Key<Texture> const &key, GLint param)
	{
		Texture2D const *texture;
		if ((texture = (Texture2D *)getTexture(key, "filterTexture2D", GL_TEXTURE_2D)) == NULL)
			return (*this);
		texture->filter(param);
		return (*this);
	}
	
	ShadingManager &ShadingManager::filterTexture2D(Key<Texture> const &key, GLint minFilter, GLint magFilter)
	{
		Texture2D const *texture;
		if ((texture = (Texture2D *)getTexture(key, "filterTexture2D", GL_TEXTURE_2D)) == NULL)
			return (*this);
		texture->filter(minFilter, magFilter);
		return (*this);
	}

	ShadingManager &ShadingManager::storageTexture2D(Key<Texture> const &key, GLint param)
	{
		Texture2D const *texture;
		if ((texture = (Texture2D *)getTexture(key, "storageTexture2D", GL_TEXTURE_2D)) == NULL)
			return (*this);
		texture->storage(param);
		return (*this);
	}

	ShadingManager &ShadingManager::storageTexture2D(Key<Texture> const &key, GLint pack, GLint unpack)
	{
		Texture2D const *texture;
		if ((texture = (Texture2D *)getTexture(key, "storageTexture2D", GL_TEXTURE_2D)) == NULL)
			return (*this);
		texture->storage(pack, unpack);
		return (*this);
	}

	ShadingManager &ShadingManager::setOptionTransferTexture2D(Key<Texture> const &key, GLint level, GLenum format, GLenum type)
	{
		Texture2D *texture;
		if ((texture = (Texture2D *)getTexture(key, "setOptionTransferTexture2D", GL_TEXTURE_2D)) == NULL)
			return (*this);
		texture->setOptionTransfer(level, format, type);
		return (*this);
	}

	ShadingManager &ShadingManager::generateMipMapTexture2D(Key<Texture> const &key)
	{
		Texture2D *texture;
		if ((texture = (Texture2D *)getTexture(key, "generateMipMapTexture2D", GL_TEXTURE_2D)) == NULL)
			return (*this);
		texture->generateMipMap();
		return (*this);
	}

	ShadingManager &ShadingManager::writeTexture(Key<Texture> const &key, void *write)
	{
		Texture2D const *texture;
		if ((texture = (Texture2D *)getTexture(key, "writeTexture", GL_TEXTURE_2D)) == NULL)
			return (*this);
		texture->write(write);
		return (*this);
	}

	ShadingManager &ShadingManager::readTexture(Key<Texture> const &key, void *read)
	{
		Texture2D const *texture;
		if ((texture = (Texture2D *)getTexture(key, "readTexture", GL_TEXTURE_2D)) == NULL)
			return (*this);
		texture->read(read);
		return (*this);
	}

	ShadingManager &ShadingManager::bindTexture(Key<Texture> const &key)
	{
		Texture const *texture;
		if ((texture = getTexture(key, "bindTexture", GL_NONE)) == NULL)
			return (*this);
		texture->bind();
		return (*this);
	}

	ShadingManager &ShadingManager::unbindTexture(Key<Texture> const &key) 
	{
		Texture const *texture;
		if ((texture = getTexture(key, "unbindTexture", GL_NONE)) == NULL)
			return (*this);
		texture->unbind();
		return (*this);
	}

	std::uint8_t ShadingManager::getMaxLevelMipMapTexture2D(Key<Texture> const &key)
	{
		Texture2D *texture;
		if ((texture = (Texture2D *)getTexture(key, "getMaxLevelMipMapTexture2D", GL_TEXTURE_2D)) == NULL)
			return (-1);
		return (texture->getMaxLevelMipMap());
	}
}