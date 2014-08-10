#include <Render/ShadingManager.hh>
#include <Render/Storage.hh>
#include <iostream>
#include <string>

namespace gl
{
	ShadingManager::ShadingManager()
	{
	}

	ShadingManager::~ShadingManager()
	{
		for (auto it = _textures.begin(); it != _textures.end(); ++it)
			delete it->second;
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
		unbindShaderToRendering(shader);
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

	Key<Uniform> ShadingManager::getShaderUniform(Key<Shader> const &key, size_t target)
	{
		Shader const *shader;
		if ((shader = getShader(key, "getShaderUniform()")) == NULL)
			return (Key<Uniform>(KEY_DESTROY));
		return (shader->getUniform(target));
	}

	Key<Uniform> ShadingManager::addShaderUniform(Key<Shader> const &key, std::string const &flag)
	{
		Shader *shader;
		if ((shader = getShader(key, "addShaderUniform(mat4)")) == NULL)
			return (Key<Uniform>(KEY_DESTROY));
		return (shader->addUniform(flag));
	}

	Key<Uniform> ShadingManager::addShaderUniform(Key<Shader> const &key, std::string const &flag, glm::mat4 const &value)
	{
		Shader *shader;
		if ((shader = getShader(key, "addShaderUniform(mat4)")) == NULL)
			return (Key<Uniform>(KEY_DESTROY));
		return (shader->addUniform(flag, value));
	}

	Key<Uniform> ShadingManager::addShaderUniform(Key<Shader> const &key, std::string const &flag, glm::mat3 const &value)
	{
		Shader *shader;
		if ((shader = getShader(key, "addShaderUniform(mat3)")) == NULL)
			return (Key<Uniform>(KEY_DESTROY));
		return (shader->addUniform(flag, value));
	}

	Key<Uniform> ShadingManager::addShaderUniform(Key<Shader> const &key, std::string const &flag, glm::vec4 const &value)
	{
		Shader *shader;
		if ((shader = getShader(key, "addShaderUniform(vec4)")) == NULL)
			return (Key<Uniform>(KEY_DESTROY));
		return (shader->addUniform(flag, value));
	}

	Key<Uniform> ShadingManager::addShaderUniform(Key<Shader> const &key, std::string const &flag, float value)
	{
		Shader *shader;
		if ((shader = getShader(key, "addShaderUniform(float)")) == NULL)
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

	Key<InputSampler> ShadingManager::addShaderInputSampler(Key<Shader> const &keyShader, std::string const &flag)
	{
		Shader *shader;
		if ((shader = getShader(keyShader, "addShaderInputSampler()")) == NULL)
			return (Key<InputSampler>(KEY_DESTROY));
		return (shader->addInputSampler(flag));
	}

	Key<InputSampler> ShadingManager::getShaderInputSampler(Key<Shader> const &keyShader, size_t target)
	{
		Shader const *shader;
		if ((shader = getShader(keyShader, "getShaderInputSampler()")) == NULL)
			return (Key<InputSampler>(KEY_DESTROY));
		return (shader->getInputSampler(target));
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
		if ((texture = getTexture(keyTexture, "setShaderSampler()")) == NULL)
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
		if (getUniformBlock(key, "rmUniformBlock") == NULL)
			return (*this);
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

	Key<InterfaceBlock> ShadingManager::getShaderInterfaceBlock(Key<Shader> const &keyShader, size_t target)
	{
		Shader const *shader;
		if ((shader = getShader(keyShader, "getShaderInterfaceBlock()")) == NULL)
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
		if ((texture = getTexture(key, "rmTexture()")) == NULL)
			return (*this);
		texture->parameter(pname, param);
		return (*this);
	}

	ShadingManager &ShadingManager::rmTexture(Key<Texture> &key)
	{
		Texture *texture;
		if ((texture = getTexture(key, "rmTexture()")) == NULL)
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
		if ((texture = getTexture(key, "getTypeTexture()")) == NULL)
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

	Texture *ShadingManager::getTexture(Key<Texture> const &key, std::string const &in)
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

	size_t ShadingManager::getRenderPassIndex(Key<RenderPass> const &key, std::string const &in)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - " + in, "key destroy", -1);
		if (_renderPass.size() == 0)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - " + in, "no uniformBlock present in pool", -1);
		if (key == _optimizeRenderPassSearch.first)
			return (_optimizeRenderPassSearch.second);
		auto &renderPassIndex = _renderPass.find(key);
		if (renderPassIndex == _renderPass.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - " + in, "uniformBlock not find", -1);
		_optimizeRenderPassSearch.first = key;
		_optimizeRenderPassSearch.second = renderPassIndex->second;
		return (renderPassIndex->second);
	}

	size_t ShadingManager::getRenderIndex(Key<Render> const &key, std::string const &in)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - " + in, "key destroy", -1);
		if (_render.size() == 0)
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - " + in, "no uniformBlock present in pool", -1);
		if (key == _optimizeRenderSearch.first)
			return (_optimizeRenderSearch.second);
		auto &renderIndex = _render.find(key);
		if (renderIndex == _render.end())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - " + in, "uniformBlock not find", -1);
		_optimizeRenderSearch.first = key;
		_optimizeRenderSearch.second = renderIndex->second;
		return (renderIndex->second);
	}

	RenderPass *ShadingManager::getRenderPass(Key<RenderPass> const &key, std::string const &in)
	{
		size_t index;
		if ((index = getRenderPassIndex(key, in)) == -1)
			return (NULL);
		return (&_renderPassPool[index]);
	}

	Render *ShadingManager::getRender(Key<Render> const &key, std::string const &in)
	{
		size_t index;
		if ((index = getRenderIndex(key, in)) == -1)
			return (NULL);
		return (&_renderPool[index]);
	}

	Material *ShadingManager::getMaterial(Key<Material> const &key, std::string const &in)
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

	ShadingManager &ShadingManager::uploadTexture(Key<Texture> const &key, GLenum format, GLenum type, GLvoid *img)
	{
		Texture const *texture;
		if ((texture = getTexture(key, "uploadTexture")) == NULL)
			return (*this);
		texture->upload(format, type, img);
		texture->generateMipMap();
		return (*this);
	}

	ShadingManager &ShadingManager::downloadTexture(Key<Texture> const &key, GLenum format, GLenum type, GLvoid *img)
	{
		Texture const *texture;
		if ((texture = getTexture(key, "downloadTexture")) == NULL)
			return (*this);
		texture->download(format, type, img);
		return (*this);
	}

	ShadingManager &ShadingManager::setlevelTargetTexture(Key<Texture> const &key, uint8_t levelTarget)
	{
		Texture *texture;
		if ((texture = getTexture(key, "levelTargetTexture")) == NULL)
			return (*this);
		texture->setLevelTarget(levelTarget);
		return (*this);
	}

	ShadingManager &ShadingManager::bindTexture(Key<Texture> const &key)
	{
		Texture const *texture;
		if ((texture = getTexture(key, "bindTexture")) == NULL)
			return (*this);
		texture->bind();
		return (*this);
	}

	ShadingManager &ShadingManager::unbindTexture(Key<Texture> const &key) 
	{
		Texture const *texture;
		if ((texture = getTexture(key, "unbindTexture")) == NULL)
			return (*this);
		texture->unbind();
		return (*this);
	}

	Key<RenderPass> ShadingManager::addRenderPass(Key<Shader> const &keyShader)
	{
		Key<RenderPass> key;

		_renderPassPool.push_back(RenderPass());
		_renderPass[key] = _renderPassPool.size() - 1;
		bindShaderRenderPass(key, keyShader);
		return (key);
	}

	Key<Render> ShadingManager::addRender(Key<Shader> const &keyShader)
	{
		Key<Render> key;

		_renderPool.push_back(Render());
		_render[key] = _renderPool.size() - 1;
		bindShaderRender(key, keyShader);
		return (key);
	}

	Key<RenderPass> ShadingManager::addRenderPass()
	{
		Key<RenderPass> key;

		_renderPassPool.push_back(RenderPass());
		_renderPass[key] = _renderPassPool.size() - 1;
		return (key);
	}

	Key<Render> ShadingManager::addRender()
	{
		Key<Render> key;

		_renderPool.push_back(Render());
		_render[key] = _renderPool.size() - 1;
		return (key);
	}

	ShadingManager &ShadingManager::rmRenderPass(Key<RenderPass> &key)
	{
		RenderPass *render;
		if ((render = getRenderPass(key, "rmRenderPass")) == NULL)
			return (*this);
		unbindRenderingInput(render);
		_renderPass.erase(key);
		key.destroy();
		return (*this);
	}

	ShadingManager &ShadingManager::rmRender(Key<Render> &key)
	{
		Render *render;
		if ((render = getRender(key, "rmRenderPass")) == NULL)
			return (*this);
		_render.erase(key);
		key.destroy();
		return (*this);
	}

	Key<RenderPass> ShadingManager::getRenderPass(size_t target) const
	{
		if (target >= _renderPass.size())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - getRenderPass(size_t target)", "the target is out of range", Key<RenderPass>(KEY_DESTROY));
		auto &element = _renderPass.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	Key<Render> ShadingManager::getRender(size_t target) const
	{
		if (target >= _render.size())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp - getRenderPass(size_t target)", "the target is out of range", Key<Render>(KEY_DESTROY));
		auto &element = _render.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	void ShadingManager::unbindShaderToRendering(Shader *s)
	{
		for (size_t index = 0; index < _bindShader.size(); ++index)
			if (_bindShader[index].s == s)
				_bindShader[index].r->bindShader(NULL);
	}

	void ShadingManager::unbindRenderingTarget(Render *r)
	{
		for (size_t index = 0; index < _bindRendering.size(); ++index)
		{
			if (_bindRendering[index].target == r)
			{
				r->unbindInput();
				return;
			}
		}
	}

	void ShadingManager::unbindRenderingInput(RenderPass *r)
	{
		for (size_t index = 0; index < _bindRendering.size(); ++index)
		{
			if (_bindRendering[index].input == r)
				_bindRendering[index].target->unbindInput();
		}
	}

	ShadingManager &ShadingManager::bindShaderRenderPass(Key<RenderPass> const &r, Key<Shader> const &s)
	{
		RenderPass *render;
		Shader *shader;

		if ((render = getRenderPass(r, "bindShaderRenderPass")) == NULL)
			return (*this);
		if ((shader = getShader(s, "bindShaderRenderPass")) == NULL)
			return (*this);
		render->bindShader(shader);
		addBindShaderToRendering(render, shader);
		return (*this);
	}

	ShadingManager &ShadingManager::bindShaderRender(Key<Render> const &r, Key<Shader> const &s)
	{
		Render *render;
		Shader *shader;

		if ((render = getRender(r, "bindShaderRender")) == NULL)
			return (*this);
		if ((shader = getShader(s, "bindShaderRender")) == NULL)
			return (*this);
		render->bindShader(shader);
		addBindShaderToRendering(render, shader);
		return (*this);
	}

	void ShadingManager::addBindShaderToRendering(Render *r, Shader *s)
	{
		for (size_t index = 0; index < _bindShader.size(); ++index)
		{
			if (_bindShader[index].s == s)
			{
				_bindShader[index].r = r;
				return ;
			}
		}
		_bindShader.push_back(BindingShader(r, s));
	}

	ShadingManager &ShadingManager::bindInputRenderPass(Key<RenderPass> const &t, Key<RenderPass> const &i)
	{
		RenderPass *target;
		RenderPass *input;
		if ((target = getRenderPass(t, "bindInputRenderPass")) == NULL)
			return (*this);
		if ((input = getRenderPass(i, "bindInputRenderPass")) == NULL)
			return (*this);
		target->bindInput(*input);
		addBindTargetToInput(target, input);
		return (*this);
	}

	ShadingManager &ShadingManager::bindInputRender(Key<Render> const &t, Key<RenderPass> const &i)
	{
		Render *target;
		RenderPass *input;
		if ((target = getRender(t, "bindInputRender")) == NULL)
			return (*this);
		if ((input = getRenderPass(i, "bindInputRender")) == NULL)
			return (*this);
		target->bindInput(*input);
		addBindTargetToInput(target, input);
		return (*this);
	}

	void ShadingManager::addBindTargetToInput(Render *t, RenderPass *i)
	{
		for (size_t index = 0; index < _bindRendering.size(); ++index)
		{
			if (_bindRendering[index].target == t)
			{
				_bindRendering[index].input = i;
				return ;
			}
		}
		_bindRendering.push_back(BindingRenderPass(t, i));
	}

	ShadingManager &ShadingManager::pushClearTaskRenderPass(Key<RenderPass> const &key, bool color, bool depth, bool stencil)
	{
		RenderPass *renderPass;
		if ((renderPass = getRenderPass(key, "setClearOptionRenderPass")) == NULL)
			return (*this);
		renderPass->pushClearTask(color, depth, stencil);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetClearValueTaskRenderPass(Key<RenderPass> const &key, glm::vec4 const &color, float depth, uint8_t stencil)
	{
		RenderPass *renderPass;
		if ((renderPass = getRenderPass(key, "setClearValueRenderPass")) == NULL)
			return (*this);
		renderPass->pushSetClearValueTask(color, depth, stencil);
		return (*this);
	}
	
	ShadingManager &ShadingManager::pushSetColorMaskTaskRenderPass(Key<RenderPass> const &key, glm::bvec4 const &color, GLuint index)
	{
		RenderPass *renderPass;
		if ((renderPass = getRenderPass(key, "setColorMaskRenderPass")) == NULL)
			return (*this);
		renderPass->pushSetColorMaskTask(index, color);
		return (*this);
	}
	
	ShadingManager &ShadingManager::pushSetStencilMaskTaskRenderPass(Key<RenderPass> const &key, uint8_t front, uint8_t back)
	{
		RenderPass *renderPass;
		if ((renderPass = getRenderPass(key, "setDepthStencilMaskRenderPass")) == NULL)
			return (*this);
		renderPass->pushSetStencilMaskTask(front, back);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetDepthMaskTaskRenderPass(Key<RenderPass> const &key, bool depth)
	{
		RenderPass *renderPass;
		if ((renderPass = getRenderPass(key, "setDepthStencilMaskRenderPass")) == NULL)
			return (*this);
		renderPass->pushSetDepthMaskTask(depth);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetTestTaskRenderPass(Key<RenderPass> const &key, bool scissor, bool stencil, bool depth)
	{
		RenderPass *renderPass;
		if ((renderPass = getRenderPass(key, "setDepthStencilMaskRenderPass")) == NULL)
			return (*this);
		renderPass->pushSetTestTask(scissor, stencil, depth);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetScissorTaskRenderPass(Key<RenderPass> const &key, glm::ivec4 const &area)
	{
		RenderPass *renderPass;
		if ((renderPass = getRenderPass(key, "setDepthStencilMaskRenderPass")) == NULL)
			return (*this);
		renderPass->pushSetScissorTask(area);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetStencilFunctionFrontFaceTaskRenderPass(Key<RenderPass> const &key, GLenum func, int ref, uint8_t mask)
	{
		RenderPass *renderPass;
		if ((renderPass = getRenderPass(key, "setStencilFunctionFrontFaceRenderPass")) == NULL)
			return (*this);
		renderPass->pushSetStencilFunctionFrontFaceTask(func, ref, mask);
		return (*this);
	}
	ShadingManager &ShadingManager::pushSetStencilOperationFrontFaceTaskRenderPass(Key<RenderPass> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		RenderPass *renderPass;
		if ((renderPass = getRenderPass(key, "setStencilOperationFrontFaceRenderPass")) == NULL)
			return (*this);
		renderPass->pushSetStencilOperationFrontFaceTask(opStencilFail, opDepthFail, opDepthPass);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetStencilFunctionBackFaceTaskRenderPass(Key<RenderPass> const &key, GLenum func, int ref, uint8_t mask)
	{
		RenderPass *renderPass;
		if ((renderPass = getRenderPass(key, "setStencilFunctionBackFaceRenderPass")) == NULL)
			return (*this);
		renderPass->pushSetStencilFunctionBackFaceTask(func, ref, mask);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetStencilOperationBackFaceTaskRenderPass(Key<RenderPass> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		RenderPass *renderPass;
		if ((renderPass = getRenderPass(key, "setStencilOperationBackFaceRenderPass")) == NULL)
			return (*this);
		renderPass->pushSetStencilOperationBackFaceTask(opStencilFail, opDepthFail, opDepthPass);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetStencilFunctionTaskRenderPass(Key<RenderPass> const &key, GLenum func, int ref, uint8_t mask)
	{
		RenderPass *renderPass;
		if ((renderPass = getRenderPass(key, "setStencilFunctionRenderPass")) == NULL)
			return (*this);
		renderPass->pushSetStencilFunctionTask(func, ref, mask);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetStencilOperationTaskRenderPass(Key<RenderPass> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		RenderPass *renderPass;
		if ((renderPass = getRenderPass(key, "setStencilOperationRenderPass")) == NULL)
			return (*this);
		renderPass->pushSetStencilOperationTask(opStencilFail, opDepthFail, opDepthPass);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetBlendEquationTaskRenderPass(Key<RenderPass> const &key, GLenum colorMode, GLenum alphaMode)
	{
		RenderPass *renderPass;
		if ((renderPass = getRenderPass(key, "setBlendEquationRenderPass")) == NULL)
			return (*this);
		renderPass->pushSetBlendEquationTask(colorMode, alphaMode);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetBlendEquationTaskRenderPass(Key<RenderPass> const &key, GLenum mode)
	{
		RenderPass *renderPass;
		if ((renderPass = getRenderPass(key, "setBlendEquationRenderPass")) == NULL)
			return (*this);
		renderPass->pushSetBlendEquationTask(mode);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetBlendFuncTaskRenderPass(Key<RenderPass> const &key, GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha)
	{
		RenderPass *renderPass;
		if ((renderPass = getRenderPass(key, "setBlendFuncRenderPass")) == NULL)
			return (*this);
		renderPass->pushSetBlendFuncTask(srcRGB, destRGB, srcAlpha, destAlpha);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetBlendFuncTaskRenderPass(Key<RenderPass> const &key, GLenum src, GLenum dest)
	{
		RenderPass *renderPass;
		if ((renderPass = getRenderPass(key, "setBlendFuncRenderPass")) == NULL)
			return (*this);
		renderPass->pushSetBlendFuncTask(src, dest);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetBlendConstantTaskRenderPass(Key<RenderPass> const &key, glm::vec4 const &blendColor)
	{
		RenderPass *renderPass;
		if ((renderPass = getRenderPass(key, "setBlendConstantRenderPass")) == NULL)
			return (*this);
		renderPass->pushSetBlendConstantTask(blendColor);
		return (*this);
	}

	ShadingManager &ShadingManager::popTaskRenderPass(Key<RenderPass> const &key)
	{
		RenderPass *renderPass;
		if ((renderPass = getRenderPass(key, "popTaskRenderPass")) == NULL)
			return (*this);
		renderPass->popTask();
		return (*this);
	}

	ShadingManager &ShadingManager::pushClearTaskRender(Key<Render> const &key, bool color, bool depth, bool stencil)
	{
		Render *render;
		if ((render = getRender(key, "setClearOptionRender")) == NULL)
			return (*this);
		render->pushClearTask(color, depth, stencil);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetClearValueTaskRender(Key<Render> const &key, glm::vec4 const &color, float depth, uint8_t stencil)
	{
		Render *render;
		if ((render = getRender(key, "setClearValueRender")) == NULL)
			return (*this);
		render->pushSetClearValueTask(color, depth, stencil);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetColorMaskTaskRender(Key<Render> const &key, glm::bvec4 const &color, GLuint index)
	{
		Render *render;
		if ((render = getRender(key, "setColorMaskRender")) == NULL)
			return (*this);
		render->pushSetColorMaskTask(index, color);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetStencilMaskTaskRender(Key<Render> const &key, uint8_t front, uint8_t back)
	{
		Render *render;
		if ((render = getRender(key, "setDepthStencilMaskRender")) == NULL)
			return (*this);
		render->pushSetStencilMaskTask(front, back);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetDepthMaskTaskRender(Key<Render> const &key, bool depth)
	{
		Render *render;
		if ((render = getRender(key, "setDepthStencilMaskRender")) == NULL)
			return (*this);
		render->pushSetDepthMaskTask(depth);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetTestTaskRender(Key<Render> const &key, bool scissor, bool stencil, bool depth)
	{
		Render *render;
		if ((render = getRender(key, "setDepthStencilMaskRender")) == NULL)
			return (*this);
		render->pushSetTestTask(scissor, stencil, depth);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetScissorTaskRender(Key<Render> const &key, glm::ivec4 const &area)
	{
		Render *render;
		if ((render = getRender(key, "setDepthStencilMaskRender")) == NULL)
			return (*this);
		render->pushSetScissorTask(area);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetStencilFunctionFrontFaceTaskRender(Key<Render> const &key, GLenum func, int ref, uint8_t mask)
	{
		Render *render;
		if ((render = getRender(key, "setStencilFunctionFrontFaceRender")) == NULL)
			return (*this);
		render->pushSetStencilFunctionFrontFaceTask(func, ref, mask);
		return (*this);
	}
	ShadingManager &ShadingManager::pushSetStencilOperationFrontFaceTaskRender(Key<Render> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepth)
	{
		Render *render;
		if ((render = getRender(key, "setStencilOperationFrontFaceRender")) == NULL)
			return (*this);
		render->pushSetStencilOperationFrontFaceTask(opStencilFail, opDepthFail, opDepth);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetStencilFunctionBackFaceTaskRender(Key<Render> const &key, GLenum func, int ref, uint8_t mask)
	{
		Render *render;
		if ((render = getRender(key, "setStencilFunctionBackFaceRender")) == NULL)
			return (*this);
		render->pushSetStencilFunctionBackFaceTask(func, ref, mask);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetStencilOperationBackFaceTaskRender(Key<Render> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepth)
	{
		Render *render;
		if ((render = getRender(key, "setStencilOperationBackFaceRender")) == NULL)
			return (*this);
		render->pushSetStencilOperationBackFaceTask(opStencilFail, opDepthFail, opDepth);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetStencilFunctionTaskRender(Key<Render> const &key, GLenum func, int ref, uint8_t mask)
	{
		Render *render;
		if ((render = getRender(key, "setStencilFunctionRender")) == NULL)
			return (*this);
		render->pushSetStencilFunctionTask(func, ref, mask);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetStencilOperationTaskRender(Key<Render> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepth)
	{
		Render *render;
		if ((render = getRender(key, "setStencilOperationRender")) == NULL)
			return (*this);
		render->pushSetStencilOperationTask(opStencilFail, opDepthFail, opDepth);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetBlendEquationTaskRender(Key<Render> const &key, GLenum colorMode, GLenum alphaMode)
	{
		Render *render;
		if ((render = getRender(key, "setBlendEquationRender")) == NULL)
			return (*this);
		render->pushSetBlendEquationTask(colorMode, alphaMode);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetBlendEquationTaskRender(Key<Render> const &key, GLenum mode)
	{
		Render *render;
		if ((render = getRender(key, "setBlendEquationRender")) == NULL)
			return (*this);
		render->pushSetBlendEquationTask(mode);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetBlendFuncTaskRender(Key<Render> const &key, GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha)
	{
		Render *render;
		if ((render = getRender(key, "setBlendFuncRender")) == NULL)
			return (*this);
		render->pushSetBlendFuncTask(srcRGB, destRGB, srcAlpha, destAlpha);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetBlendFuncTaskRender(Key<Render> const &key, GLenum src, GLenum dest)
	{
		Render *render;
		if ((render = getRender(key, "setBlendFuncRender")) == NULL)
			return (*this);
		render->pushSetBlendFuncTask(src, dest);
		return (*this);
	}

	ShadingManager &ShadingManager::pushSetBlendConstantTaskRender(Key<Render> const &key, glm::vec4 const &blendColor)
	{
		Render *render;
		if ((render = getRender(key, "setBlendConstantRender")) == NULL)
			return (*this);
		render->pushSetBlendConstantTask(blendColor);
		return (*this);
	}

	ShadingManager &ShadingManager::popTaskRender(Key<Render> const &key)
	{
		Render *render;
		if ((render = getRender(key, "popTaskRender")) == NULL)
			return (*this);
		render->popTask();
		return (*this);
	}

	ShadingManager &ShadingManager::configRender(Key<Render> const &r, glm::ivec4 const &rect)
	{
		Render *render;

		if ((render = getRender(r, "configRender")) == NULL)
			return (*this);
		render->configRect(rect);
		return (*this);
	}

	ShadingManager &ShadingManager::setModeRender(Key<Render> const &r, GLenum mode)
	{
		Render *render;

		if ((render = getRender(r, "configRender")) == NULL)
			return (*this);
		render->setMode(mode);
		return (*this);
	}

	ShadingManager &ShadingManager::addColorOutputRenderPass(Key<RenderPass> const &key, GLenum target, GLenum internalFormat)
	{
		RenderPass *renderPass;
		if ((renderPass = getRenderPass(key, "addColorOutputRenderPass")) == NULL)
			return (*this);
		renderPass->addColorOutput(target, internalFormat);
		return (*this);
	}

	ShadingManager &ShadingManager::setModeRenderPass(Key<RenderPass> const &key, GLenum mode)
	{
		RenderPass *renderPass;

		if ((renderPass = getRenderPass(key, "setModeRenderPass")) == NULL)
			return (*this);
		renderPass->setMode(mode);
		return (*this);
	}

	ShadingManager &ShadingManager::draw(AGE::Vector<AGE::Drawable> const &objectRender)
	{
		for (size_t index = 0; index < _renderPassPool.size(); ++index)
		{
			_renderPassPool[index].update();
			for (size_t index = 0; index < objectRender.size(); ++index)
			{
				Material *material;
				if ((material = getMaterial(objectRender[index].material, "draw")) == NULL)
					return (*this);
				RenderPass *renderPass = material->get<Render_Pass>();
				renderPass->accessShader()->preDraw(*material, objectRender[index].transformation);
				geometryManager.draw(renderPass->getMode(), objectRender[index].mesh.indices, objectRender[index].mesh.vertices);
			}
		}
		for (size_t index = 0; index < _renderPool.size(); ++index)
		{
			Render &render = _renderPool[index];
			render.update();
			for (size_t index = 0; index < objectRender.size(); ++index)
			{
				Material *material;
				if ((material = getMaterial(objectRender[index].material, "draw")) == NULL)
					return (*this);
			//	RenderPass *renderPass = material->get<Render_Pass>();
			//	renderPass->accessShader()->preDraw(*material, objectRender[index].transformation);
				render.accessShader()->preDraw(*material, objectRender[index].transformation);
				geometryManager.draw(render.getMode(), objectRender[index].mesh.indices, objectRender[index].mesh.vertices);
			}
		}
		return (*this);
	}

	Key<Material> ShadingManager::addMaterial()
	{
		Key<Material> key;

		_materials[key];
		return (key);
	}

	ShadingManager &ShadingManager::rmMaterial(Key<Material> &key)
	{
		Material *material;
		if ((material = getMaterial(key, "rmMaterial()")) == NULL)
			return (*this);
		_materials.erase(key);
		key.destroy();
		return (*this);
	}

	Key<Material> ShadingManager::getMaterial(size_t target) const
	{
		if (target >= _materials.size())
			DEBUG_MESSAGE("Warning", "ShadingManager.cpp-getMaterial(size_t target)", "the target is out of range", Key<Material>(KEY_DESTROY));
		auto &element = _materials.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	ShadingManager &ShadingManager::setRenderPassMaterial(Key<Material> const &m, Key<RenderPass> const &r)
	{
		RenderPass *renderPass;
		Material *material;

		if ((renderPass = getRenderPass(r, "setRenderPassMaterial")) == NULL)
			return (*this);
		if ((material = getMaterial(m, "setRenderPassMaterial")) == NULL)
			return (*this);
		material->set<Render_Pass>(renderPass);
		return (*this);
	}

	ShadingManager &ShadingManager::unbindMaterialToShader(Key<Shader> const &shaderKey, Key<Uniform> const &uniformKey)
	{
		Shader *shader;
		if ((shader = getShader(shaderKey, "unbindMaterialToShader")) == NULL)
			return (*this);
		shader->unbindMaterial(uniformKey);
		return (*this);
	}

	ShadingManager &ShadingManager::bindTransformationToShader(Key<Shader> const &shaderKey, Key<Uniform> const &uniformKey)
	{
		Shader *shader;
		if ((shader = getShader(shaderKey, "unbindMaterialToShader")) == NULL)
			return (*this);
		shader->bindingTransformation(uniformKey);
		return (*this);
	}

	ShadingManager &ShadingManager::configRenderPass(Key<RenderPass> const &key, glm::ivec4 const &rect, GLint sample)
	{
		RenderPass *renderPass;

		if ((renderPass = getRenderPass(key, "configRenderPass")) == NULL)
			return (*this);
		renderPass->configRect(rect);
		renderPass->configSample(sample);
		return (*this);
	}


}