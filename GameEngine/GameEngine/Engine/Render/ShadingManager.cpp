#include <Render/ShadingManager.hh>
#include <Render/Storage.hh>
#include <iostream>
#include <string>

namespace gl
{

	ShadingManager::ShadingManager()
		: _preShaderQuad(NULL)
	{
	}

	ShadingManager::~ShadingManager()
	{
		if (_preShaderQuad == NULL)
			delete _preShaderQuad;
		for (auto it = _shaders.begin(); it != _shaders.end(); ++it)
			delete it->second;
		for (auto it = _textures.begin(); it != _textures.end(); ++it)
			delete it->second;
		for (auto it = _renderPass.begin(); it != _renderPass.end(); ++it)
			delete it->second;
		for (auto it = _renderPostEffect.begin(); it != _renderPostEffect.end(); ++it)
			delete it->second;
	}

	ShadingManager &ShadingManager::createPreShaderQuad()
	{
		if (_preShaderQuad != NULL)
			return (*this);
		if ((_preShaderQuad = Shader::createPreShaderQuad()) == NULL)
			assert(0);
		_preShaderQuad->addSampler("input_sampler");
		return (*this);
	}

	Key<Shader> ShadingManager::addComputeShader(std::string const &compute)
	{
		Key<Shader> key;
		Shader *shader;

		if ((shader = Shader::createComputeShader(compute)) == NULL)
			assert(0);
		_shaders[key] = shader;
		return (key);
	}

	Key<Shader> ShadingManager::addShader(std::string const &vertex, std::string const &frag)
	{
		Key<Shader> key;
		Shader *shader;

		if ((shader = Shader::createShader(vertex, frag)) == NULL)
			assert(0);
		_shaders[key] = shader;
		return (key);
	}

	Key<Shader> ShadingManager::addShader(std::string const &geo, std::string const &vertex, std::string const &frag)
	{
		Key<Shader> key;
		Shader *shader;

		if ((shader = Shader::createShader(vertex, frag, geo)) == NULL)
			assert(0);
		_shaders[key] = shader;
		return (key);
	}

	Key<Shader> ShadingManager::getShader(size_t target) const
	{
		if (target >= _shaders.size())
			assert(0);
		auto &element = _shaders.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	Key<Uniform> ShadingManager::getShaderUniform(Key<Shader> const &key, size_t target)
	{
		Shader const *shader;
		if ((shader = getShader(key)) == NULL)
			return (Key<Uniform>(KEY_DESTROY));
		return (shader->getUniform(target));
	}

	Key<Uniform> ShadingManager::addShaderUniform(Key<Shader> const &key, std::string const &flag)
	{
		Shader *shader;
		if ((shader = getShader(key)) == NULL)
			return (Key<Uniform>(KEY_DESTROY));
		return (shader->addUniform(flag));
	}

	Key<Uniform> ShadingManager::addShaderUniform(Key<Shader> const &key, std::string const &flag, glm::mat4 const &value)
	{
		Shader *shader;
		if ((shader = getShader(key)) == NULL)
			return (Key<Uniform>(KEY_DESTROY));
		return (shader->addUniform(flag, value));
	}

	Key<Uniform> ShadingManager::addShaderUniform(Key<Shader> const &key, std::string const &flag, glm::mat3 const &value)
	{
		Shader *shader;
		if ((shader = getShader(key)) == NULL)
			return (Key<Uniform>(KEY_DESTROY));
		return (shader->addUniform(flag, value));
	}

	Key<Uniform> ShadingManager::addShaderUniform(Key<Shader> const &key, std::string const &flag, glm::vec4 const &value)
	{
		Shader *shader;
		if ((shader = getShader(key)) == NULL)
			return (Key<Uniform>(KEY_DESTROY));
		return (shader->addUniform(flag, value));
	}

	Key<Uniform> ShadingManager::addShaderUniform(Key<Shader> const &key, std::string const &flag, float value)
	{
		Shader *shader;
		if ((shader = getShader(key)) == NULL)
			return (Key<Uniform>(KEY_DESTROY));
		return (shader->addUniform(flag, value));
	}

	ShadingManager &ShadingManager::setShaderUniform(Key<Shader> const &keyShader, Key<Uniform> const &key, glm::mat4 const &mat4)
	{
		Shader *shader;
		if ((shader = getShader(keyShader)) == NULL)
			return (*this);
		shader->setUniform(key, mat4);
		return (*this);
	}

	ShadingManager &ShadingManager::setShaderUniform(Key<Shader> const &keyShader, Key<Uniform> const &key, glm::vec4 const &vec4)
	{
		Shader *shader;
		if ((shader = getShader(keyShader)) == NULL)
			return (*this);
		shader->setUniform(key, vec4);
		return (*this);
	}

	ShadingManager &ShadingManager::setShaderUniform(Key<Shader> const &keyShader, Key<Uniform> const &key, float v)
	{
		Shader *shader;
		if ((shader = getShader(keyShader)) == NULL)
			return (*this);
		shader->setUniform(key, v);
		return (*this);
	}

	ShadingManager &ShadingManager::setShaderUniform(Key<Shader> const &keyShader, Key<Uniform> const &key, glm::mat3 const &mat3)
	{
		Shader *shader;
		if ((shader = getShader(keyShader)) == NULL)
			return (*this);
		shader->setUniform(key, mat3);
		return (*this);
	}

	Key<Sampler> ShadingManager::addShaderSampler(Key<Shader> const &keyShader, std::string const &flag)
	{
		Shader *shader;
		if ((shader = getShader(keyShader)) == NULL)
			return (Key<Sampler>(KEY_DESTROY));
		return (shader->addSampler(flag));
	}

	Key<Sampler> ShadingManager::getShaderSampler(Key<Shader> const &keyShader, size_t target)
	{
		Shader const *shader;
		if ((shader = getShader(keyShader)) == NULL)
			return (Key<Sampler>(KEY_DESTROY));
		return (shader->getSampler(target));
	}

	ShadingManager &ShadingManager::setShaderSampler(Key<Shader> const &keyShader, Key<Sampler> const &keySampler, Key<Texture> const &keyTexture)
	{
		Shader *shader;
		if ((shader = getShader(keyShader)) == NULL)
			return (*this);
		Texture *texture;
		if ((texture = getTexture(keyTexture)) == NULL)
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
		if (getUniformBlock(key) == NULL)
			return (*this);
		_uniformBlock.erase(key);
		key.destroy();
		return (*this);
	}

	Key<UniformBlock> ShadingManager::getUniformBlock(size_t target) const
	{
		if (target >= _uniformBlock.size())
			assert(0);
		auto &element = _uniformBlock.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	Key<InterfaceBlock> ShadingManager::addShaderInterfaceBlock(Key<Shader> const &keyShader, std::string const &flag, Key<UniformBlock> const &keyUniformBlock)
	{
		Shader *shader;
		if ((shader = getShader(keyShader)) == NULL)
			return (Key<InterfaceBlock>(KEY_DESTROY));
		UniformBlock *uniformBlock;
		if ((uniformBlock = getUniformBlock(keyUniformBlock)) == NULL)
			return (Key<InterfaceBlock>(KEY_DESTROY));
		return (shader->addInterfaceBlock(flag, *uniformBlock));
	}

	Key<InterfaceBlock> ShadingManager::getShaderInterfaceBlock(Key<Shader> const &keyShader, size_t target)
	{
		Shader const *shader;
		if ((shader = getShader(keyShader)) == NULL)
			return (Key<InterfaceBlock>(KEY_DESTROY));
		return (shader->getInterfaceBlock(target));
	}

	Key<Texture> ShadingManager::addTexture2D(GLsizei width, GLsizei height, GLenum internalFormat, bool mipmapping)
	{
		Key<Texture> key;

		_textures[key] = new Texture2D(width, height, internalFormat, mipmapping);
		return (key);
	}

	ShadingManager &ShadingManager::parameterTexture(Key<Texture> const &key, GLenum pname, GLint param)
	{
		Texture *texture;
		if ((texture = getTexture(key)) == NULL)
			return (*this);
		texture->parameter(pname, param);
		return (*this);
	}

	ShadingManager &ShadingManager::rmTexture(Key<Texture> &key)
	{
		Texture *texture;
		if ((texture = getTexture(key)) == NULL)
			return (*this);
		delete texture;
		_textures.erase(key);
		key.destroy();
		return (*this);
	}

	Key<Texture> ShadingManager::getTexture(size_t target) const
	{
		if (target >= _textures.size())
			assert(0);
		auto &element = _textures.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	GLenum ShadingManager::getTypeTexture(Key<Texture> const &key)
	{
		Texture const *texture;
		if ((texture = getTexture(key)) == NULL)
			return (GL_NONE);
		return (texture->getType());
	}

	Shader *ShadingManager::getShader(Key<Shader> const &key)
	{
		if (!key)
			assert(0);
		if (_shaders.size() == 0)
			assert(0);
		if (key == _optimizeShaderSearch.first)
			return (_optimizeShaderSearch.second);
		auto &shader = _shaders.find(key);
		if (shader == _shaders.end())
			assert(0);
		_optimizeShaderSearch.first = key;
		_optimizeShaderSearch.second = shader->second;
		return (shader->second);
	}

	Texture *ShadingManager::getTexture(Key<Texture> const &key)
	{
		if (!key)
			assert(0);
		if (_textures.size() == 0)
			assert(0);
		if (key == _optimizeTextureSearch.first)
			return (_optimizeTextureSearch.second);
		auto &texture = _textures.find(key);
		if (texture == _textures.end())
			assert(0);
		_optimizeTextureSearch.first = key;
		_optimizeTextureSearch.second = texture->second;
		return (texture->second);
	}

	UniformBlock *ShadingManager::getUniformBlock(Key<UniformBlock> const &key)
	{
		if (!key)
			assert(0);
		if (_uniformBlock.size() == 0)
			assert(0);
		if (key == _optimizeUniformBlockSearch.first)
			return (_optimizeUniformBlockSearch.second);
		auto &uniformBlock = _uniformBlock.find(key);
		if (uniformBlock == _uniformBlock.end())
			assert(0);
		_optimizeUniformBlockSearch.first = key;
		_optimizeUniformBlockSearch.second = &uniformBlock->second;
		return (&uniformBlock->second);
	}

	RenderPass *ShadingManager::getRenderPass(Key<RenderPass> const &key)
	{
		if (!key)
			assert(0);
		if (_renderPass.size() == 0)
			assert(0);
		if (key == _optimizeRenderPassSearch.first)
			return (_optimizeRenderPassSearch.second);
		auto &renderPassIndex = _renderPass.find(key);
		if (renderPassIndex == _renderPass.end())
			assert(0);
		_optimizeRenderPassSearch.first = key;
		_optimizeRenderPassSearch.second = renderPassIndex->second;
		return (renderPassIndex->second);
	}


	ShadingManager &ShadingManager::uploadTexture(Key<Texture> const &key, GLenum format, GLenum type, GLvoid *img)
	{
		Texture const *texture;
		if ((texture = getTexture(key)) == NULL)
			return (*this);
		texture->upload(format, type, img);
		texture->generateMipMap();
		return (*this);
	}

	ShadingManager &ShadingManager::downloadTexture(Key<Texture> const &key, GLenum format, GLenum type, GLvoid *img)
	{
		Texture const *texture;
		if ((texture = getTexture(key)) == NULL)
			return (*this);
		texture->download(format, type, img);
		return (*this);
	}

	ShadingManager &ShadingManager::setlevelTargetTexture(Key<Texture> const &key, uint8_t levelTarget)
	{
		Texture *texture;
		if ((texture = getTexture(key)) == NULL)
			return (*this);
		texture->setLevelTarget(levelTarget);
		return (*this);
	}

	ShadingManager &ShadingManager::bindTexture(Key<Texture> const &key)
	{
		Texture const *texture;
		if ((texture = getTexture(key)) == NULL)
			return (*this);
		texture->bind();
		return (*this);
	}

	ShadingManager &ShadingManager::unbindTexture(Key<Texture> const &key) 
	{
		Texture const *texture;
		if ((texture = getTexture(key)) == NULL)
			return (*this);
		texture->unbind();
		return (*this);
	}

	Key<RenderPass> ShadingManager::addRenderPass(Key<Shader> const &keyShader, glm::ivec4 const &rect)
	{
		Key<RenderPass> key;
		Shader *shader;

		if ((shader = getShader(keyShader)) == NULL)
			return (Key<RenderPass>(KEY_DESTROY));
		auto &element = _renderPass[key] = new RenderPass(*shader, geometryManager, materialManager);
		element->configRect(rect);
		return (key);
	}

	Key<RenderPass> ShadingManager::getRenderPass(size_t target) const
	{
		if (target >= _renderPass.size())
			assert(0);
		auto &element = _renderPass.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	GEN_DEF_RENDER_PUSH_TASK(RenderPass);

	ShadingManager &ShadingManager::configRenderPass(Key<RenderPass> const &key, glm::ivec4 const &rect, GLenum mode, GLint sample)
	{
		RenderPass *renderPass;

		if ((renderPass = getRenderPass(key)) == NULL)
			return (*this);
		renderPass->setMode(mode);
		renderPass->configRect(rect);
		return (*this);
	}

	ShadingManager &ShadingManager::pushOutputColorRenderPass(Key<RenderPass> const &key, GLenum attachement, GLenum internalFormat)
	{
		RenderPass *renderPass;

		if ((renderPass = getRenderPass(key)) == NULL)
			return (*this);
		renderPass->pushColorOutput(attachement, internalFormat);
		return (*this);
	}

	ShadingManager &ShadingManager::popOutputColorRenderPass(Key<RenderPass> const &key)
	{
		RenderPass *renderPass;

		if ((renderPass = getRenderPass(key)) == NULL)
			return (*this);
		renderPass->popColorOutput();
		return (*this);
	}

	ShadingManager &ShadingManager::pushInputColorRenderPass(Key<RenderPass> const &key, Key<Sampler> const &s)
	{
		RenderPass *renderPass;

		if ((renderPass = getRenderPass(key)) == NULL)
			return (*this);
		renderPass->pushInputSampler(s);
		return (*this);
	}
	
	ShadingManager &ShadingManager::popInputColorRenderPass(Key<RenderPass> const &key)
	{
		RenderPass *renderPass;

		if ((renderPass = getRenderPass(key)) == NULL)
			return (*this);
		renderPass->popInputSampler();
		return (*this);
	}

	ShadingManager &ShadingManager::createDepthBufferRenderPass(Key<RenderPass> const &key)
	{
		RenderPass *renderPass;

		if ((renderPass = getRenderPass(key)) == NULL)
			return (*this);
		renderPass->createDepthBuffer();
		return (*this);
	}

	ShadingManager &ShadingManager::createStencilBufferRenderPass(Key<RenderPass> const &key)
	{
		RenderPass *renderPass;

		if ((renderPass = getRenderPass(key)) == NULL)
			return (*this);
		renderPass->createStencilBuffer();
		return (*this);
	}

	ShadingManager &ShadingManager::useInputDepthRenderPass(Key<RenderPass> const &key)
	{
		RenderPass *renderPass;

		if ((renderPass = getRenderPass(key)) == NULL)
			return (*this);
		renderPass->useInputDepth();
		return (*this);
	}

	ShadingManager &ShadingManager::unUseInputDepthRenderPass(Key<RenderPass> const &key)
	{
		RenderPass *renderPass;

		if ((renderPass = getRenderPass(key)) == NULL)
			return (*this);
		renderPass->unUseInputDepth();
		return (*this);
	}

	ShadingManager &ShadingManager::useInputStencilRenderPass(Key<RenderPass> const &key)
	{
		RenderPass *renderPass;

		if ((renderPass = getRenderPass(key)) == NULL)
			return (*this);
		renderPass->useInputStencil();
		return (*this);
	}

	ShadingManager &ShadingManager::unUseInputStencilRenderPass(Key<RenderPass> const &key)
	{
		RenderPass *renderPass;

		if ((renderPass = getRenderPass(key)) == NULL)
			return (*this);
		renderPass->unUseInputStencil();
		return (*this);
	}

	ShadingManager &ShadingManager::useInputColorRenderPass(Key<RenderPass> const &key, GLenum attachement)
	{
		RenderPass *renderPass;

		if ((renderPass = getRenderPass(key)) == NULL)
			return (*this);
		renderPass->useInputColor(attachement);
		return (*this);
	}

	ShadingManager &ShadingManager::unUseInputColorRenderPass(Key<RenderPass> const &key, GLenum attachement)
	{
		RenderPass *renderPass;

		if ((renderPass = getRenderPass(key)) == NULL)
			return (*this);
		renderPass->unUseInputColor(attachement);
		return (*this);
	}

	ShadingManager &ShadingManager::draw(AGE::Vector<AGE::Drawable> const &objectRender)
	{
		auto &element = _renderPass.begin();
		element->second->setRenderPassObjects(objectRender);
		element->second->draw();
		auto &elementOnScreen = _renderOnScreen.begin();
		elementOnScreen->second->draw();
		return (*this);
	}

	ShadingManager &ShadingManager::unbindMaterialToShader(Key<Shader> const &shaderKey, Key<Uniform> const &uniformKey)
	{
		Shader *shader;
		if ((shader = getShader(shaderKey)) == NULL)
			return (*this);
		shader->unbindMaterial(uniformKey);
		return (*this);
	}

	ShadingManager &ShadingManager::bindTransformationToShader(Key<Shader> const &shaderKey, Key<Uniform> const &uniformKey)
	{
		Shader *shader;
		if ((shader = getShader(shaderKey)) == NULL)
			return (*this);
		shader->bindingTransformation(uniformKey);
		return (*this);
	}

	RenderPostEffect *ShadingManager::getRenderPostEffect(Key<RenderPostEffect> const &key)
	{
		if (!key)
			assert(0);
		if (_renderPostEffect.size() == 0)
			assert(0);
		if (key == _optimizeRenderPostEffectSearch.first)
			return (_optimizeRenderPostEffectSearch.second);
		auto &renderPostEffect = _renderPostEffect.find(key);
		if (renderPostEffect == _renderPostEffect.end())
			assert(0);
		_optimizeRenderPostEffectSearch.first = key;
		_optimizeRenderPostEffectSearch.second = renderPostEffect->second;
		return (renderPostEffect->second);
	}

	RenderOnScreen *ShadingManager::getRenderOnScreen(Key<RenderOnScreen> const &key)
	{
		if (!key)
			assert(0);
		if (_renderOnScreen.size() == 0)
			assert(0);
		if (key == _optimizeRenderOnScreenSearch.first)
			return (_optimizeRenderOnScreenSearch.second);
		auto &renderOnScreen = _renderOnScreen.find(key);
		if (renderOnScreen == _renderOnScreen.end())
			assert(0);
		_optimizeRenderOnScreenSearch.first = key;
		_optimizeRenderOnScreenSearch.second = renderOnScreen->second;
		return (renderOnScreen->second);
	}

	Key<RenderPostEffect> ShadingManager::addRenderPostEffect(Key<Shader> const &s, glm::ivec4 const &rect)
	{
		Key<RenderPostEffect> key;
		Shader *shader;

		if ((shader = getShader(s)) == NULL)
			return  (Key<RenderPostEffect>(KEY_DESTROY));
		auto &element = _renderPostEffect[key] = new RenderPostEffect(geometryManager.getSimpleForm(QUAD), *shader, geometryManager);
		element->pushInputSampler(_preShaderQuad->getSampler(0));
		element->configRect(rect);
		return (key);
	}

	Key<RenderPostEffect> ShadingManager::getRenderPostEffect(size_t target) const
	{
		if (target >= _renderPass.size())
			assert(0);
		auto &element = _renderPostEffect.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	GEN_DEF_RENDER_PUSH_TASK(RenderPostEffect);

	ShadingManager &ShadingManager::configRenderPostEffect(Key<RenderPostEffect> const &key, glm::ivec4 const &rect, GLenum mode, GLint sample)
	{
		RenderPostEffect *renderPostEffect;

		if ((renderPostEffect = getRenderPostEffect(key)) == NULL)
			return (*this);
		renderPostEffect->setMode(mode);
		renderPostEffect->configRect(rect);
		return (*this);
	}

	ShadingManager &ShadingManager::pushOutputColorRenderPostEffect(Key<RenderPostEffect> const &key, GLenum attachement, GLenum internalFormat)
	{
		RenderPostEffect *renderPostEffect;

		if ((renderPostEffect = getRenderPostEffect(key)) == NULL)
			return (*this);
		renderPostEffect->pushColorOutput(attachement, internalFormat);
		return (*this);
	}

	ShadingManager & ShadingManager::popOutputColorRenderPostEffect(Key<RenderPostEffect> const &key)
	{
		RenderPostEffect *renderPostEffect;

		if ((renderPostEffect = getRenderPostEffect(key)) == NULL)
			return (*this);
		renderPostEffect->popColorOutput();
		return (*this);
	}

	ShadingManager &ShadingManager::pushInputColorRenderPostEffect(Key<RenderPostEffect> const &key, Key<Sampler> const &s)
	{
		RenderPostEffect *renderPostEffect;

		if ((renderPostEffect = getRenderPostEffect(key)) == NULL)
			return (*this);
		renderPostEffect->pushInputSampler(s);
		return (*this);
	}
	
	ShadingManager &ShadingManager::popInputColorRenderPostEffect(Key<RenderPostEffect> const &key)
	{
		RenderPostEffect *renderPostEffect;

		if ((renderPostEffect = getRenderPostEffect(key)) == NULL)
			return (*this);
		renderPostEffect->popInputSampler();
		return (*this);
	}

	Key<RenderOnScreen> ShadingManager::addRenderOnScreen(glm::ivec4 const &rect)
	{
		Key<RenderOnScreen> key;

		createPreShaderQuad();
		auto &element = _renderOnScreen[key] = new RenderOnScreen(geometryManager.getSimpleForm(QUAD), *_preShaderQuad, geometryManager);
		element->pushInputSampler(_preShaderQuad->getSampler(0));
		element->configRect(rect);
		return (key);
	}

	Key<RenderOnScreen> ShadingManager::getRenderOnScreen(size_t target) const
	{
		if (target >= _textures.size())
			assert(0);
		auto &element = _renderOnScreen.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	GEN_DEF_RENDER_PUSH_TASK(RenderOnScreen);

	ShadingManager &ShadingManager::branch(Key<RenderPass> const &from, Key<RenderPass> const &to)
	{
		RenderPass *renderPassFrom;
		RenderPass *renderPassTo;

		if ((renderPassFrom = getRenderPass(from)) == NULL)
			return (*this);
		if ((renderPassTo = getRenderPass(to)) == NULL)
			return (*this);
		renderPassTo->branchInput(*renderPassFrom);
		return (*this);
	}

	ShadingManager &ShadingManager::branch(Key<RenderPass> const &from, Key<RenderPostEffect> const &to)
	{
		RenderPass *renderPassFrom;
		RenderPostEffect *renderPassTo;

		if ((renderPassFrom = getRenderPass(from)) == NULL)
			return (*this);
		if ((renderPassTo = getRenderPostEffect(to)) == NULL)
			return (*this);
		renderPassTo->branchInput(*renderPassFrom);
		return (*this);
	}

	ShadingManager &ShadingManager::branch(Key<RenderPass> const &from, Key<RenderOnScreen> const &to)
	{
		RenderPass *renderPassFrom;
		RenderOnScreen *renderPassTo;

		if ((renderPassFrom = getRenderPass(from)) == NULL)
			return (*this);
		if ((renderPassTo = getRenderOnScreen(to)) == NULL)
			return (*this);
		renderPassTo->branchInput(*renderPassFrom);
		return (*this);
	}

	ShadingManager &ShadingManager::branch(Key<RenderPostEffect> const &from, Key<RenderOnScreen> const &to)
	{
		RenderPostEffect *renderPassFrom;
		RenderOnScreen *renderPassTo;

		if ((renderPassFrom = getRenderPostEffect(from)) == NULL)
			return (*this);
		if ((renderPassTo = getRenderOnScreen(to)) == NULL)
			return (*this);
		renderPassTo->branchInput(*renderPassFrom);
		return (*this);
	}
}