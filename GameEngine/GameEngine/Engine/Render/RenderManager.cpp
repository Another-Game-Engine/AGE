#include <Render/RenderManager.hh>
#include <Render/Storage.hh>
#include <Render/Pipeline.hh>
#include <algorithm>
#include <iostream>
#include <string>

namespace gl
{

	RenderManager::RenderManager()
		: _preShaderQuad(NULL),
		_defaultMaterialCreated(false),
		_defaultTexture2DCreated(false),
		_renderManagerNumber(0)
	{
		static size_t id = 0;
		_renderManagerNumber = id++;
	}

	RenderManager::~RenderManager()
	{
		if (_preShaderQuad == NULL)
			delete _preShaderQuad;
		for (auto &shader : _shaders)
			delete shader;
		for (auto &uniformBlock : _uniformBlock)
			delete uniformBlock;
		for (auto texture : _textures)
			delete texture;
		for (auto it = _renderPass.begin(); it != _renderPass.end(); ++it)
			delete it->second;
		for (auto it = _renderPostEffect.begin(); it != _renderPostEffect.end(); ++it)
			delete it->second;
	}

	RenderManager &RenderManager::createPreShaderQuad()
	{
		if (_preShaderQuad != NULL)
			return (*this);
		if ((_preShaderQuad = Shader::createPreShaderQuad(_materials)) == NULL)
			assert(0);
		_preShaderQuad->addSampler("input_sampler");
		return (*this);
	}

	Key<Shader> RenderManager::addComputeShader(std::string const &compute)
	{
		Key<Shader> key = Key<Shader>::createKey(_renderManagerNumber);
		Shader *shader = Shader::createComputeShader(compute, _materials);
		if (_shaders.size() <= key.getId())
			_shaders.push_back(NULL);
		_shaders[key.getId()] = shader;
		return (key);
	}

	Key<Shader> RenderManager::addShader(std::string const &vertex, std::string const &frag)
	{
		Key<Shader> key = Key<Shader>::createKey(_renderManagerNumber);
		Shader *shader = Shader::createShader(vertex, frag, _materials);
		if (_shaders.size() <= key.getId())
			_shaders.push_back(NULL);
		_shaders[key.getId()] = shader;
		return (key);
	}

	Key<Shader> RenderManager::addShader(std::string const &geo, std::string const &vertex, std::string const &frag)
	{
		Key<Shader> key = Key<Shader>::createKey(_renderManagerNumber);
		Shader *shader = Shader::createShader(vertex, frag, geo, _materials);
		if (_shaders.size() <= key.getId())
			_shaders.push_back(NULL);
		_shaders[key.getId()] = shader;
		return (key);
	}

	Key<Uniform> RenderManager::addShaderUniform(Key<Shader> const &key, std::string const &flag)
	{
		Shader *shader = getShader(key);
		return (shader->addUniform(flag));
	}


	Key<Uniform> RenderManager::addShaderUniform(Key<Shader> const &key, std::string const &flag, glm::mat4 const &value)
	{
		Shader *shader = getShader(key);
		return (shader->addUniform(flag, value));
	}

	Key<Uniform> RenderManager::addShaderUniform(Key<Shader> const &key, std::string const &flag, glm::mat3 const &value)
	{
		Shader *shader = getShader(key);
		return (shader->addUniform(flag, value));
	}

	Key<Uniform> RenderManager::addShaderUniform(Key<Shader> const &key, std::string const &flag, glm::vec3 const &value)
	{
		Shader *shader = getShader(key);
		return (shader->addUniform(flag, value));
	}

	Key<Uniform> RenderManager::addShaderUniform(Key<Shader> const &key, std::string const &flag, glm::vec4 const &value)
	{
		Shader *shader = getShader(key);
		return (shader->addUniform(flag, value));
	}


	Key<Uniform> RenderManager::addShaderUniform(Key<Shader> const &key, std::string const &flag, float value)
	{
		Shader *shader = getShader(key);
		return (shader->addUniform(flag, value));
	}

	RenderManager &RenderManager::setShaderUniform(Key<Shader> const &keyShader, Key<Uniform> const &key, glm::mat4 const &mat4)
	{
		Shader *shader = getShader(keyShader);
		shader->setUniform(key, mat4);
		return (*this);
	}

	RenderManager &RenderManager::setShaderUniform(Key<Shader> const &keyShader, Key<Uniform> const &key, glm::vec4 const &vec4)
	{
		Shader *shader = getShader(keyShader);
		shader->setUniform(key, vec4);
		return (*this);
	}

	RenderManager &RenderManager::setShaderUniform(Key<Shader> const &keyShader, Key<Uniform> const &key, glm::vec3 const &vec3)
	{
		Shader *shader = getShader(keyShader);
		shader->setUniform(key, vec3);
		return (*this);
	}

	RenderManager &RenderManager::setShaderUniform(Key<Shader> const &keyShader, Key<Uniform> const &key, float v)
	{
		Shader *shader;
		if ((shader = getShader(keyShader)) == NULL)
			return (*this);
		shader->setUniform(key, v);
		return (*this);
	}

	RenderManager &RenderManager::setShaderUniform(Key<Shader> const &keyShader, Key<Uniform> const &key, glm::mat3 const &mat3)
	{
		Shader *shader;
		if ((shader = getShader(keyShader)) == NULL)
			return (*this);
		shader->setUniform(key, mat3);
		return (*this);
	}

	Key<Sampler> RenderManager::addShaderSampler(Key<Shader> const &keyShader, std::string const &flag)
	{
		Shader *shader = getShader(keyShader);
		return (shader->addSampler(flag));
	}

	RenderManager &RenderManager::setShaderSampler(Key<Shader> const &keyShader, Key<Sampler> const &keySampler, Key<Texture> const &keyTexture)
	{
		Shader *shader = getShader(keyShader);
		Texture *texture = getTexture(keyTexture);
		shader->setSampler(keySampler, *texture);
		return (*this);
	}

	Key<UniformBlock> RenderManager::addUniformBlock()
	{
		Key<UniformBlock> key = Key<UniformBlock>::createKey(_renderManagerNumber);
		if (_uniformBlock.size() <= key.getId())
			_uniformBlock.push_back(NULL);
		_uniformBlock[key.getId()] = new UniformBlock();
		return (key);
	}

	RenderManager &RenderManager::introspectionBlock(Key<Shader> const &s, Key<InterfaceBlock> const &i, Key<UniformBlock> const &u)
	{
		Shader *shader = getShader(s);
		UniformBlock *uniformBlock = getUniformBlock(u);
		shader->introspection(i, *uniformBlock);
		return (*this);
	}

	Key<InterfaceBlock> RenderManager::addShaderInterfaceBlock(Key<Shader> const &keyShader, std::string const &flag, Key<UniformBlock> &keyUniformBlock)
	{
		Shader *shader = getShader(keyShader);
		UniformBlock *uniformBlock = getUniformBlock(keyUniformBlock);
		return (shader->addInterfaceBlock(flag, *uniformBlock));
	}

	RenderManager &RenderManager::setShaderInterfaceBlock(Key<Shader> const &keyShader, Key<InterfaceBlock> const &i, Key<UniformBlock> const &u)
	{
		Shader *shader = getShader(keyShader);
		UniformBlock *uniformBlock = getUniformBlock(u);
		shader->setInterfaceBlock(i, *uniformBlock);
		return (*this);
	}

	Key<InterfaceBlock> RenderManager::getShaderInterfaceBlock(Key<Shader> const &keyShader, size_t target)
	{
		Shader const *shader = getShader(keyShader);
		return (shader->getInterfaceBlock(target));
	}

	Key<Texture> RenderManager::addTexture2D(GLsizei width, GLsizei height, GLenum internalFormat, bool mipmapping)
	{
		Key<Texture> key = Key<Texture>::createKey(_renderManagerNumber);
		if (_textures.size() <= key.getId())
			_textures.push_back(NULL);
		_textures[key.getId()] = new Texture2D(width, height, internalFormat, mipmapping);
		return (key);
	}

	RenderManager &RenderManager::parameterTexture(Key<Texture> const &key, GLenum pname, GLint param)
	{
		Texture *texture;
		if ((texture = getTexture(key)) == NULL)
			return (*this);
		texture->parameter(pname, param);
		return (*this);
	}

	GLenum RenderManager::getTypeTexture(Key<Texture> const &key)
	{
		Texture const *texture;
		if ((texture = getTexture(key)) == NULL)
			return (GL_NONE);
		return (texture->getType());
	}

	Shader *RenderManager::getShader(Key<Shader> const &key)
	{
		assert(key);
		return (_shaders[key.getId()]);
	}

	Texture *RenderManager::getTexture(Key<Texture> const &key)
	{
		assert(key);
		return (_textures[key.getId()]);
	}

	UniformBlock *RenderManager::getUniformBlock(Key<UniformBlock> const &key)
	{
		assert(key);
		return (_uniformBlock[key.getId()]);
	}

	RenderManager &RenderManager::uploadTexture(Key<Texture> const &key, GLenum format, GLenum type, GLvoid *img)
	{
		Texture const *texture = getTexture(key);
		texture->upload(format, type, img);
		texture->generateMipMap();
		return (*this);
	}

	RenderManager &RenderManager::downloadTexture(Key<Texture> const &key, GLenum format, GLenum type, GLvoid *img)
	{
		Texture const *texture;
		if ((texture = getTexture(key)) == NULL)
			return (*this);
		texture->download(format, type, img);
		return (*this);
	}

	RenderManager &RenderManager::setlevelTargetTexture(Key<Texture> const &key, uint8_t levelTarget)
	{
		Texture *texture;
		if ((texture = getTexture(key)) == NULL)
			return (*this);
		texture->setLevelTarget(levelTarget);
		return (*this);
	}

	RenderManager &RenderManager::bindTexture(Key<Texture> const &key)
	{
		Texture const *texture;
		if ((texture = getTexture(key)) == NULL)
			return (*this);
		texture->bind();
		return (*this);
	}

	RenderManager &RenderManager::unbindTexture(Key<Texture> const &key) 
	{
		Texture const *texture;
		if ((texture = getTexture(key)) == NULL)
			return (*this);
		texture->unbind();
		return (*this);
	}

	Key<RenderPass> RenderManager::addRenderPass(Key<Shader> const &keyShader, glm::ivec4 const &rect)
	{
		Key<RenderPass> key = Key<RenderPass>::createKey();
		Shader *shader = getShader(keyShader);
		auto &element = _renderPass[key] = new RenderPass(*shader, *this);
		element->configRect(rect);
		return (key);
	}

	Key<RenderPass> RenderManager::getRenderPass(size_t target) const
	{
		if (target >= _renderPass.size())
			assert(0);
		auto &element = _renderPass.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	GEN_DEF_RENDER_PUSH_TASK(RenderPass);
	GEN_DEF_RENDEROFFSCREEN_PUSH_TASK(RenderPass);
	GEN_DEF_DRAWABLERENDER_PUSH_TASK(RenderPass);

	RenderManager &RenderManager::pushDrawTaskRenderBuffer(Key<RenderPass> const &key)
	{
		RenderPass *renderPass = getRenderPass(key);
		renderPass->pushPassTask();
		return (*this);
	}

	RenderManager &RenderManager::unbindMaterialToShader(Key<Shader> const &shaderKey, Key<Uniform> const &uniformKey)
	{
		Shader *shader;
		if ((shader = getShader(shaderKey)) == NULL)
			return (*this);
		shader->unbindMaterial(uniformKey);
		return (*this);
	}

	RenderManager &RenderManager::bindTransformationToShader(Key<Shader> const &shaderKey, Key<Uniform> const &uniformKey)
	{
		Shader *shader;
		if ((shader = getShader(shaderKey)) == NULL)
			return (*this);
		shader->bindingTransformation(uniformKey);
		return (*this);
	}

	GEN_DEF_SEARCH_FUNCTION(EmptyRenderPass, _emptyRenderPass);
	GEN_DEF_SEARCH_FUNCTION(RenderPass, _renderPass);
	GEN_DEF_SEARCH_FUNCTION(RenderOnScreen, _renderOnScreen);
	GEN_DEF_SEARCH_FUNCTION(RenderPostEffect, _renderPostEffect);

	size_t RenderManager::getIndexPipeline(Key<Pipeline> const &key)
	{
		if (!key)
			assert(0);
		if (_pipelines.size() == 0)
			assert(0);
		if (key == _optimizePipelineSearch.first)
			return (_optimizePipelineSearch.second);
		auto &pipeline = _pipelines.find(key);
		if (pipeline == _pipelines.end())
			assert(0);
		_optimizePipelineSearch.first = key;
		_optimizePipelineSearch.second = pipeline->second;
		return (pipeline->second);
	}

	Pipeline *RenderManager::getPipeline(Key<Pipeline> const &key)
	{
		return (&_pipelineOrdered[getIndexPipeline(key)]);
	}

	Key<RenderPostEffect> RenderManager::addRenderPostEffect(Key<Shader> const &s, glm::ivec4 const &rect)
	{
		Key<RenderPostEffect> key = Key<RenderPostEffect>::createKey();

		geometryManager.createQuadSimpleForm();
		Shader *shader = getShader(s);
		auto &element = _renderPostEffect[key] = new RenderPostEffect(*shader, *this);
		element->configRect(rect);
		return (key);
	}

	Key<RenderPostEffect> RenderManager::getRenderPostEffect(size_t target) const
	{
		if (target >= _renderPass.size())
			assert(0);
		auto &element = _renderPostEffect.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	GEN_DEF_RENDER_PUSH_TASK(RenderPostEffect);
	GEN_DEF_RENDEROFFSCREEN_PUSH_TASK(RenderPostEffect);
	GEN_DEF_DRAWABLERENDER_PUSH_TASK(RenderPostEffect);

	Key<RenderOnScreen> RenderManager::addRenderOnScreen(glm::ivec4 const &rect, Key<RenderPass> const &r)
	{
		Key<RenderOnScreen> key = Key<RenderOnScreen>::createKey();
		RenderPass *renderPass = getRenderPass(r);
		createPreShaderQuad();
		geometryManager.createQuadSimpleForm();
		auto &element = _renderOnScreen[key] = new RenderOnScreen(*_preShaderQuad, *this);
		element->pushInputSampler(Key<Sampler>::createKeyWithId(0), GL_COLOR_ATTACHMENT0, *renderPass);
		element->configRect(rect);
		return (key);
	}

	Key<RenderOnScreen> RenderManager::addRenderOnScreen(glm::ivec4 const &rect, Key<RenderPostEffect> const &r)
	{
		Key<RenderOnScreen> key = Key<RenderOnScreen>::createKey();
		RenderPostEffect *renderPostEffect = getRenderPostEffect(r);
		createPreShaderQuad();
		geometryManager.createQuadSimpleForm();
		auto &element = _renderOnScreen[key] = new RenderOnScreen(*_preShaderQuad, *this);
		element->pushInputSampler(Key<Sampler>::createKeyWithId(0), GL_COLOR_ATTACHMENT0, *renderPostEffect);
		element->configRect(rect);
		return (key);
	}

	Key<RenderOnScreen> RenderManager::addRenderOnScreen(glm::ivec4 const &rect, Key<EmptyRenderPass> const &r)
	{
		Key<RenderOnScreen> key = Key<RenderOnScreen>::createKey();
		EmptyRenderPass *renderPostEffect = getEmptyRenderPass(r);
		createPreShaderQuad();
		geometryManager.createQuadSimpleForm();
		auto &element = _renderOnScreen[key] = new RenderOnScreen(*_preShaderQuad, *this);
		element->pushInputSampler(Key<Sampler>::createKeyWithId(0), GL_COLOR_ATTACHMENT0, *renderPostEffect);
		element->configRect(rect);
		return (key);
	}
	
	Key<RenderOnScreen> RenderManager::getRenderOnScreen(size_t target) const
	{
		if (target >= _textures.size())
			assert(0);
		auto &element = _renderOnScreen.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	GEN_DEF_RENDER_PUSH_TASK(RenderOnScreen);

	Key<EmptyRenderPass> RenderManager::addEmptyRenderPass(glm::ivec4 const &rect)
	{
		Key<EmptyRenderPass> key = Key<EmptyRenderPass>::createKey();
		
		auto &element = _emptyRenderPass[key] = new EmptyRenderPass(*this);
		element->configRect(rect);
		return (key);
	}

	Key<EmptyRenderPass> RenderManager::getEmptyRenderPass(size_t target) const
	{
		if (target >= _emptyRenderPass.size())
			assert(0);
		auto &element = _emptyRenderPass.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	GEN_DEF_RENDER_PUSH_TASK(EmptyRenderPass);
	GEN_DEF_RENDEROFFSCREEN_PUSH_TASK(EmptyRenderPass);


	Key<Pipeline> RenderManager::addPipeline()
	{
		Key<Pipeline> key = Key<Pipeline>::createKey();
	
		_pipelineOrdered.push_back(Pipeline());
		_pipelines[key] = _pipelineOrdered.size() - 1;
		return (key);
	}

	RenderManager &RenderManager::pushRenderPassPipeline(Key<Pipeline> const &p, Key<RenderPass> const &r)
	{
		Pipeline *pipeline = getPipeline(p);
		RenderPass *renderPass = getRenderPass(r);
		pipeline->pushRenderPass(renderPass);
		return (*this);
	}

	RenderManager &RenderManager::pushRenderPostEffectPipeline(Key<Pipeline> const &p, Key<RenderPostEffect> const &r)
	{
		Pipeline *pipeline = getPipeline(p);
		RenderPostEffect *render = getRenderPostEffect(r);
		pipeline->pushRender(render);
		return (*this);
	}

	RenderManager &RenderManager::pushRenderOnScreenPipeline(Key<Pipeline> const &p, Key<RenderOnScreen> const &r)
	{
		Pipeline *pipeline = getPipeline(p);
		RenderOnScreen *render = getRenderOnScreen(r);
		pipeline->pushRender(render);
		return (*this);
	}

	RenderManager &RenderManager::pushEmptyRenderPassPipeline(Key<Pipeline> const &p, Key<EmptyRenderPass> const &r)
	{
		Pipeline *pipeline = getPipeline(p);
		EmptyRenderPass *render = getEmptyRenderPass(r);
		pipeline->pushRender(render);
		return (*this);
	}
	
	Key<Pipeline> RenderManager::getPipeline(size_t target)
	{
		if (target >= _pipelines.size())
			assert(0);
		auto &element = _pipelines.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	RenderManager &RenderManager::configPipeline(Key<Pipeline> const &p, DrawType type)
	{
		Pipeline *pipeline = getPipeline(p);
		pipeline->config(type);
		return (*this);
	}

	RenderManager &RenderManager::updatePipeline(Key<Pipeline> const &p, AGE::Vector<AGE::Drawable> const &objectRender)
	{
		Pipeline *pipeline = getPipeline(p);
		pipeline->update(objectRender);
		return (*this);
	}

	RenderManager &RenderManager::drawPipelines()
	{
		for (size_t index = 0; index < _pipelineOrdered.size(); ++index)
			_pipelineOrdered[index].draw();
		return (*this);
	}

	RenderManager &RenderManager::drawPipeline(Key<Pipeline> const &key, AGE::Vector<AGE::Drawable> const &objectRender)
	{
		Pipeline *pipeline = getPipeline(key);
		pipeline->update(objectRender);
		pipeline->draw();
		return (*this);
	}

	RenderManager &RenderManager::draw(Key<RenderOnScreen> const &o, Key<RenderPass> const &r, AGE::Vector<AGE::Drawable> const &objectRender)
	{
		RenderOnScreen *renderOnScreen = getRenderOnScreen(o);
		RenderPass *renderPass = getRenderPass(r);
		renderPass->setPass(objectRender, 0, objectRender.size());
		renderPass->render();
		renderOnScreen->render();
		return (*this);
	}

	Key<Material> RenderManager::getDefaultMaterial()
	{
		if (_defaultMaterialCreated == false)
		{
			_defaultMaterial = addMaterial();
			_defaultMaterialCreated = true;
		}
		return (_defaultMaterial);
	}

	Key<Texture> RenderManager::getDefaultTexture2D()
	{
		if (_defaultTexture2DCreated == false)
		{
			_defaultTexture2D = addTexture2D(2, 2, GL_RGBA, false);
			_defaultTexture2DCreated = true;
		}
		return (_defaultTexture2D);
	}

	Key<Material> RenderManager::addMaterial()
	{
		Key<Material> key = Key<Material>::createKey(_renderManagerNumber);
		if (_materials.size() <= key.getId())
			_materials.push_back(Material());
		return (key);
	}

	Material *RenderManager::getMaterial(Key<Material> const &key)
	{
		assert(!!key);
		return (&_materials[key.getId()]);
	}

	RenderManager &RenderManager::setShaderByMaterial(Key<Shader> &keyShader, Key<Material> const &key)
	{
		Shader &shader = *getShader(keyShader);
		return (*this);
	}

}