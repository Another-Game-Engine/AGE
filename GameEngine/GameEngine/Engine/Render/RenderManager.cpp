#include <Render/RenderManager.hh>
#include <Render/Storage.hh>
#include <algorithm>
#include <iostream>
#include <string>
#include <Render/Pool.hh>
#include <Render/SimpleFormGeometry.hh>
#include <Render/Data.hh>

	typedef std::pair<uint32_t, uint32_t>				idxPair_t;
	typedef std::map<idxPair_t, uint32_t>				idxHash_t;

	static uint32_t getMiddlePoint(std::vector<glm::vec3> &vertexTab, idxHash_t &middlePoints, uint32_t p1, uint32_t p2)
	{
		// first check if we have it already
		bool firstIsSmaller = p1 < p2;
		uint32_t smallerIndex = firstIsSmaller ? p1 : p2;
		uint32_t greaterIndex = firstIsSmaller ? p2 : p1;
		idxPair_t key;

		key.first = smallerIndex;
		key.second = greaterIndex;

		uint32_t ret;

		idxHash_t::iterator it = middlePoints.find(key);
		if (it != middlePoints.end())
		{
			return it->second;
		}

		// not in cache, calculate it
		glm::vec3 point1 = vertexTab[p1];
		glm::vec3 point2 = vertexTab[p2];
		glm::vec3 middle = (point1 + point2) / 2.0f;

		// add vertex makes sure point is on unit sphere
		vertexTab.push_back(glm::normalize(glm::vec3(middle)));

		ret = uint32_t(vertexTab.size() - 1);

		// store it, return index
		middlePoints[key] = ret;
		return ret;
	}

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
		for (auto &renderPass : _renderPass)
			delete renderPass;
		for (auto &renderPostEffect : _renderPostEffect)
			delete renderPostEffect;
		for (auto &emptyRenderPass : _emptyRenderPass)
			delete emptyRenderPass;
		for (auto &renderOnScreen : _renderOnScreen)
			delete renderOnScreen;
	}

	RenderManager &RenderManager::createPreShaderQuad()
	{
		if (_preShaderQuad != NULL)
			return (*this);
		_preShaderQuad = Program::createPreShaderQuad(_materials);
		_preShaderQuad->addSampler("input_sampler");
		return (*this);
	}

	Key<Program> RenderManager::addComputeShader(std::string const &compute)
	{
		Key<Program> key = Key<Program>::createKey(_renderManagerNumber);
		Program *shader = Program::createComputeShader(compute, _materials);
		if (_shaders.size() <= key.getId())
			_shaders.push_back(NULL);
		_shaders[key.getId()] = shader;
		return (key);
	}

	Key<Program> RenderManager::addShader(std::string const &vertex, std::string const &frag)
	{
		Key<Program> key = Key<Program>::createKey(_renderManagerNumber);
		Program *shader = Program::createShader(vertex, frag, _materials);
		if (_shaders.size() <= key.getId())
			_shaders.push_back(NULL);
		_shaders[key.getId()] = shader;
		return (key);
	}

	Key<Program> RenderManager::addShader(std::string const &geo, std::string const &vertex, std::string const &frag)
	{
		Key<Program> key = Key<Program>::createKey(_renderManagerNumber);
		Program *shader = Program::createShader(vertex, frag, geo, _materials);
		if (_shaders.size() <= key.getId())
			_shaders.push_back(NULL);
		_shaders[key.getId()] = shader;
		return (key);
	}

	Key<Uniform> RenderManager::addShaderUniform(Key<Program> const &key, std::string const &flag)
	{
		Program *shader = getShader(key);
		return (shader->addUniform(flag));
	}


	Key<Uniform> RenderManager::addShaderUniform(Key<Program> const &key, std::string const &flag, glm::mat4 const &value)
	{
		Program *shader = getShader(key);
		return (shader->addUniform(flag, value));
	}

	Key<Uniform> RenderManager::addShaderUniform(Key<Program> const &key, std::string const &flag, glm::mat3 const &value)
	{
		Program *shader = getShader(key);
		return (shader->addUniform(flag, value));
	}

	Key<Uniform> RenderManager::addShaderUniform(Key<Program> const &key, std::string const &flag, glm::vec3 const &value)
	{
		Program *shader = getShader(key);
		return (shader->addUniform(flag, value));
	}

	Key<Uniform> RenderManager::addShaderUniform(Key<Program> const &key, std::string const &flag, bool value)
	{
		Program *shader = getShader(key);
		return (shader->addUniform(flag, value));
	}

	Key<Uniform> RenderManager::addShaderUniform(Key<Program> const &key, std::string const &flag, size_t sizeType, size_t size)
	{
		Program *shader = getShader(key);
		return (shader->addUniform(flag, sizeType, size));
	}

	Key<Uniform> RenderManager::addShaderUniform(Key<Program> const &key, std::string const &flag, glm::vec4 const &value)
	{
		Program *shader = getShader(key);
		return (shader->addUniform(flag, value));
	}


	Key<Uniform> RenderManager::addShaderUniform(Key<Program> const &key, std::string const &flag, float value)
	{
		Program *shader = getShader(key);
		return (shader->addUniform(flag, value));
	}

	RenderManager &RenderManager::setShaderUniform(Key<Program> const &keyShader, Key<Uniform> const &key, glm::mat4 const &mat4)
	{
		Program *shader = getShader(keyShader);
		shader->setUniform(key, mat4);
		return (*this);
	}

	RenderManager &RenderManager::setShaderUniform(Key<Program> const &keyShader, Key<Uniform> const &key, bool b)
	{
		Program *shader = getShader(keyShader);
		shader->setUniform(key, b);
		return (*this);
	}

	RenderManager &RenderManager::setShaderUniform(Key<Program> const &keyShader, Key<Uniform> const &key, glm::vec4 const &vec4)
	{
		Program *shader = getShader(keyShader);
		shader->setUniform(key, vec4);
		return (*this);
	}

	RenderManager &RenderManager::setShaderUniform(Key<Program> const &keyShader, Key<Uniform> const &key, glm::vec3 const &vec3)
	{
		Program *shader = getShader(keyShader);
		shader->setUniform(key, vec3);
		return (*this);
	}

	RenderManager &RenderManager::setShaderUniform(Key<Program> const &keyShader, Key<Uniform> const &key, float v)
	{
		Program *shader = getShader(keyShader);
		shader->setUniform(key, v);
		return (*this);
	}

	RenderManager &RenderManager::setShaderUniform(Key<Program> const &keyShader, Key<Uniform> const &key, glm::mat3 const &mat3)
	{
		Program *shader = getShader(keyShader);
		shader->setUniform(key, mat3);
		return (*this);
	}

	RenderManager &RenderManager::setShaderUniform(Key<Program> const &keyShader, Key<Uniform> const &key, glm::mat4 const &mat, size_t index)
	{
		Program *shader = getShader(keyShader);
		shader->setUniform(key, mat, index);
		return (*this);
	}

	Key<Sampler> RenderManager::addShaderSampler(Key<Program> const &keyShader, std::string const &flag)
	{
		Program *shader = getShader(keyShader);
		return (shader->addSampler(flag));
	}

	RenderManager &RenderManager::setShaderSampler(Key<Program> const &keyShader, Key<Sampler> const &keySampler, Key<Texture> const &keyTexture)
	{
		Program *shader = getShader(keyShader);
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

	RenderManager &RenderManager::introspectionBlock(Key<Program> const &s, Key<InterfaceBlock> const &i, Key<UniformBlock> const &u)
	{
		Program *shader = getShader(s);
		UniformBlock *uniformBlock = getUniformBlock(u);
		shader->introspection(i, *uniformBlock);
		return (*this);
	}

	Key<InterfaceBlock> RenderManager::addShaderInterfaceBlock(Key<Program> const &keyShader, std::string const &flag, Key<UniformBlock> &keyUniformBlock)
	{
		Program *shader = getShader(keyShader);
		UniformBlock *uniformBlock = getUniformBlock(keyUniformBlock);
		return (shader->addInterfaceBlock(flag, *uniformBlock));
	}

	RenderManager &RenderManager::setShaderInterfaceBlock(Key<Program> const &keyShader, Key<InterfaceBlock> const &i, Key<UniformBlock> const &u)
	{
		Program *shader = getShader(keyShader);
		UniformBlock *uniformBlock = getUniformBlock(u);
		shader->setInterfaceBlock(i, *uniformBlock);
		return (*this);
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
		Texture *texture= getTexture(key);
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

	Key<RenderPass> RenderManager::addRenderPass(Key<Program> const &keyShader, glm::ivec4 const &rect)
	{
		Key<RenderPass> key = Key<RenderPass>::createKey(_renderManagerNumber);
		Program *shader = getShader(keyShader);
		if (_renderPass.size() <= key.getId())
			_renderPass.push_back(new RenderPass(*shader, keyShader, *this));
		_renderPass[key.getId()]->configRect(rect);
		return (key);
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

	RenderManager &RenderManager::unbindMaterialToShader(Key<Program> const &shaderKey, Key<Uniform> const &uniformKey)
	{
		Program *shader;
		if ((shader = getShader(shaderKey)) == NULL)
			return (*this);
		shader->unbindMaterial(uniformKey);
		return (*this);
	}

	RenderManager &RenderManager::bindTransformationToShader(Key<Program> const &shaderKey, Key<Uniform> const &uniformKey)
	{
		Program *shader;
		if ((shader = getShader(shaderKey)) == NULL)
			return (*this);
		shader->bindingTransformation(uniformKey);
		return (*this);
	}

	Key<RenderPostEffect> RenderManager::addRenderPostEffect(Key<Program> const &s, glm::ivec4 const &rect)
	{
		Key<RenderPostEffect> key = Key<RenderPostEffect>::createKey(_renderManagerNumber);

		createQuadSimpleForm();
		Program *shader = getShader(s);
		if (_renderPostEffect.size() <= key.getId())
			_renderPostEffect.push_back(new RenderPostEffect(*shader, *this));
		_renderPostEffect[key.getId()]->configRect(rect);
		return (key);
	}

	GEN_DEF_RENDER_PUSH_TASK(RenderPostEffect);
	GEN_DEF_RENDEROFFSCREEN_PUSH_TASK(RenderPostEffect);
	GEN_DEF_DRAWABLERENDER_PUSH_TASK(RenderPostEffect);

	Key<RenderOnScreen> RenderManager::addRenderOnScreen(glm::ivec4 const &rect, Key<RenderPass> const &r)
	{
		Key<RenderOnScreen> key = Key<RenderOnScreen>::createKey(_renderManagerNumber);
		RenderPass *renderPass = getRenderPass(r);
		createPreShaderQuad();
		createQuadSimpleForm();
		if (_renderOnScreen.size() <= key.getId())
			_renderOnScreen.push_back(new RenderOnScreen(*_preShaderQuad, *this));
		_renderOnScreen[key.getId()]->pushInputSampler(Key<Sampler>::createKeyWithId(0), GL_COLOR_ATTACHMENT0, *renderPass);
		_renderOnScreen[key.getId()]->configRect(rect);
		return (key);
	}

	Key<RenderOnScreen> RenderManager::addRenderOnScreen(glm::ivec4 const &rect, Key<RenderPostEffect> const &r)
	{
		Key<RenderOnScreen> key = Key<RenderOnScreen>::createKey();
		RenderPostEffect *renderPostEffect = getRenderPostEffect(r);
		createPreShaderQuad();
		createQuadSimpleForm();
		if (_renderOnScreen.size() <= key.getId())
			_renderOnScreen.push_back(new RenderOnScreen(*_preShaderQuad, *this));
		_renderOnScreen[key.getId()]->pushInputSampler(Key<Sampler>::createKeyWithId(0), GL_COLOR_ATTACHMENT0, *renderPostEffect);
		_renderOnScreen[key.getId()]->configRect(rect);
		return (key);
	}

	Key<RenderOnScreen> RenderManager::addRenderOnScreen(glm::ivec4 const &rect, Key<EmptyRenderPass> const &r)
	{
		Key<RenderOnScreen> key = Key<RenderOnScreen>::createKey();
		EmptyRenderPass *renderPostEffect = getEmptyRenderPass(r);
		createPreShaderQuad();
		createQuadSimpleForm();
		if (_renderOnScreen.size() <= key.getId())
			_renderOnScreen.push_back(new RenderOnScreen(*_preShaderQuad, *this));
		_renderOnScreen[key.getId()]->pushInputSampler(Key<Sampler>::createKeyWithId(0), GL_COLOR_ATTACHMENT0, *renderPostEffect);
		_renderOnScreen[key.getId()]->configRect(rect);
		return (key);
	}

	GEN_DEF_RENDER_PUSH_TASK(RenderOnScreen);

	Key<EmptyRenderPass> RenderManager::addEmptyRenderPass(glm::ivec4 const &rect)
	{
		Key<EmptyRenderPass> key = Key<EmptyRenderPass>::createKey(_renderManagerNumber);
		
		if (_emptyRenderPass.size() <= key.getId())
			_emptyRenderPass.push_back(new EmptyRenderPass(*this));
		_emptyRenderPass[key.getId()]->configRect(rect);
		return (key);
	}

	GEN_DEF_RENDER_PUSH_TASK(EmptyRenderPass);
	GEN_DEF_RENDEROFFSCREEN_PUSH_TASK(EmptyRenderPass);


	Key<Pipeline> RenderManager::addPipeline()
	{
		Key<Pipeline> key = Key<Pipeline>::createKey(_renderManagerNumber);
	
		if (_pipelines.size() <= key.getId())
			_pipelines.push_back(Pipeline());
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
		for (size_t index = 0; index < _pipelines.size(); ++index)
			_pipelines[index].draw();
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

	RenderManager &RenderManager::updateShader(Key<Program> const &shader)
	{
		Program &s = *getShader(shader);
		s.use();
		s.update();
		return (*this);
	}

	RenderManager &RenderManager::updateShader(Key<Program> const &shader, glm::mat4 const &transform, Key<Material> const &material)
	{
		Program &s = *getShader(shader);
		Material &m = *getMaterial(material);
		s.use();
		s.update(transform, m);
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

	RenderManager &RenderManager::setShaderByMaterial(Key<Program> &keyShader, Key<Material> const &key)
	{
		Program &shader = *getShader(keyShader);
		return (*this);
	}

	void RenderManager::generateIcoSphere(size_t recursion, AGE::Vector<void *> &vertex, AGE::Vector<unsigned int> &indices, size_t &nbrElementId, size_t &nbrElementGeo)
	{
		idxHash_t					middlePoints;
		AGE::Vector<glm::vec3>		vertexTab;
		AGE::Vector<glm::u32vec3>	idTab;
		uint32_t					currentIdx = 0;

		// create 12 vertices of a icosahedron
		float t = (1.0f + std::sqrt(5.0f)) / 2.0f;

		vertexTab.push_back(glm::normalize(glm::vec3(-1, t, 0)));
		vertexTab.push_back(glm::normalize(glm::vec3(1, t, 0)));
		vertexTab.push_back(glm::normalize(glm::vec3(-1, -t, 0)));
		vertexTab.push_back(glm::normalize(glm::vec3(1, -t, 0)));

		vertexTab.push_back(glm::normalize(glm::vec3(0, -1, t)));
		vertexTab.push_back(glm::normalize(glm::vec3(0, 1, t)));
		vertexTab.push_back(glm::normalize(glm::vec3(0, -1, -t)));
		vertexTab.push_back(glm::normalize(glm::vec3(0, 1, -t)));

		vertexTab.push_back(glm::normalize(glm::vec3(t, 0, -1)));
		vertexTab.push_back(glm::normalize(glm::vec3(t, 0, 1)));
		vertexTab.push_back(glm::normalize(glm::vec3(-t, 0, -1)));
		vertexTab.push_back(glm::normalize(glm::vec3(-t, 0, 1)));

		idTab.push_back(glm::u32vec3(0, 11, 5));
		idTab.push_back(glm::u32vec3(0, 5, 1));
		idTab.push_back(glm::u32vec3(0, 1, 7));
		idTab.push_back(glm::u32vec3(0, 7, 10));
		idTab.push_back(glm::u32vec3(0, 10, 11));

		// 5 adjacent faces 
		idTab.push_back(glm::u32vec3(1, 5, 9));
		idTab.push_back(glm::u32vec3(5, 11, 4));
		idTab.push_back(glm::u32vec3(11, 10, 2));
		idTab.push_back(glm::u32vec3(10, 7, 6));
		idTab.push_back(glm::u32vec3(7, 1, 8));

		// 5 faces around point 3
		idTab.push_back(glm::u32vec3(3, 9, 4));
		idTab.push_back(glm::u32vec3(3, 4, 2));
		idTab.push_back(glm::u32vec3(3, 2, 6));
		idTab.push_back(glm::u32vec3(3, 6, 8));
		idTab.push_back(glm::u32vec3(3, 8, 9));

		// 5 adjacent faces 
		idTab.push_back(glm::u32vec3(4, 9, 5));
		idTab.push_back(glm::u32vec3(2, 4, 11));
		idTab.push_back(glm::u32vec3(6, 2, 10));
		idTab.push_back(glm::u32vec3(8, 6, 7));
		idTab.push_back(glm::u32vec3(9, 8, 1));

		// refine triangles
		for (int i = 0; i < recursion; i++)
		{
			std::vector<glm::u32vec3>			idTab2;

			for (auto indices : idTab)
			{
				// replace triangle by 4 triangles
				uint32_t a = getMiddlePoint(vertexTab, middlePoints, indices.x, indices.y);
				uint32_t b = getMiddlePoint(vertexTab, middlePoints, indices.y, indices.z);
				uint32_t c = getMiddlePoint(vertexTab, middlePoints, indices.z, indices.x);

				idTab2.push_back(glm::u32vec3(indices.x, a, c));
				idTab2.push_back(glm::u32vec3(indices.y, b, a));
				idTab2.push_back(glm::u32vec3(indices.z, c, b));
				idTab2.push_back(glm::u32vec3(a, b, c));
			}
			idTab = idTab2;
		}
		vertex[0] = new uint8_t[vertexTab.size() * sizeof(glm::vec3)];
		for (size_t index = 0; index < vertexTab.size(); ++index)
		{
			((float *)vertex[0])[index] = vertexTab[index].x;
			((float *)vertex[0])[index + 1] = vertexTab[index].y;
			((float *)vertex[0])[index + 2] = vertexTab[index].z;
		}
		indices.resize(idTab.size() * 3);
		for (size_t index = 0; index < idTab.size(); ++index)
		{
			indices[index] = idTab[0].x;
			indices[index + 1] = idTab[1].y;
			indices[index + 2] = idTab[2].z;
		}
		nbrElementGeo = vertexTab.size();
		nbrElementId = idTab.size();
	}

	void RenderManager::initSimpleForm()
	{
		if (simpleFormPoolGeo.getId() == -1)
		{
			simpleFormPoolGeo = addVertexPool(1, { GL_FLOAT }, { sizeof(float) }, { 3 });
		}
		if (simpleFormPoolId.getId() == -1)
			simpleFormPoolId = addIndexPool();
	}

	RenderManager &RenderManager::createQuadSimpleForm()
	{
		auto &element = _simpleFormGeo.find(SimpleForm::QUAD);
		if (element != _simpleFormGeo.end())
			return (*this);
		initSimpleForm();
		_simpleFormGeo[SimpleForm::QUAD] = addVertices(4, { 4 * 3 * 4 }, { &quadForm[0] }, simpleFormPoolGeo);
		_simpleFormId[SimpleForm::QUAD] = addIndices(6, quadFormId, simpleFormPoolId);
		return (*this);
	}

	RenderManager &RenderManager::createSphereSimpleForm()
	{
		auto &element = _simpleFormGeo.find(SimpleForm::SPHERE);
		if (element != _simpleFormGeo.end())
			return (*this);
		initSimpleForm();
		size_t const nbrBuffer = 1;
		std::vector<void *> buffer = { NULL };
		std::vector<unsigned int> id;
		size_t nbrElementGeo;
		size_t nbrElementId;
		generateIcoSphere(1, buffer, id, nbrElementId, nbrElementGeo);
		size_t sizeBuffer = 4 * 3 * nbrElementGeo;
		_simpleFormGeo[SimpleForm::SPHERE] = addVertices(nbrElementGeo, { 4 * 3 * nbrElementGeo }, buffer, simpleFormPoolGeo);
		_simpleFormId[SimpleForm::SPHERE] = addIndices(nbrElementId, id, simpleFormPoolId);
		if (buffer[0] != NULL)
			delete buffer[0];
		return (*this);
	}


	Key<Vertices> RenderManager::getSimpleFormGeo(SimpleForm form)
	{
		auto key = _simpleFormGeo.find(form);
		assert(key != _simpleFormGeo.end());
		return (key->second);
	}

	Key<Indices> RenderManager::getSimpleFormId(SimpleForm form)
	{
		auto key = _simpleFormId.find(form);
		assert(key != _simpleFormId.end());
		return (key->second);
	}

	Key<IndexPool> RenderManager::addIndexPool()
	{
		Key<IndexPool> key = Key<IndexPool>::createKey();
		if (key.getId() >= _indexPool.size())
			_indexPool.push_back(NULL);
		_indexPool[key.getId()] = new IndexPool();
		return (key);
	}

	Key<VertexPool> RenderManager::addVertexPool(uint8_t nbrAttributes, AGE::Vector<GLenum> const &typeComponent, AGE::Vector<uint8_t> const &sizeTypeComponent, AGE::Vector<uint8_t> const &nbrComponent)
	{
		Key<VertexPool> key = Key<VertexPool>::createKey();
		if (key.getId() >= _vertexPool.size())
			_vertexPool.push_back(NULL);
		_vertexPool[key.getId()] = new VertexPool(nbrAttributes, typeComponent, sizeTypeComponent, nbrComponent);
		return (key);
	}

	Key<Vertices> RenderManager::addVertices(size_t nbrVertices, AGE::Vector<size_t> const &sizeBuffers, AGE::Vector<void *> const &buffers, Key<VertexPool> const &pool)
	{
		auto p = getVertexPool(pool);
		return (p->addElementPool(Data(nbrVertices, sizeBuffers, buffers)));
	}

	Key<Indices> RenderManager::addIndices(size_t nbrIndices, AGE::Vector<uint32_t> const &buffer, Key<IndexPool> const &pool)
	{
		auto p = getIndexPool(pool);
		return (p->addElementPool(Data(nbrIndices, buffer)));
	}

	RenderManager &RenderManager::rmVertices(Key<VertexPool> const &vertexPool, Key<Vertices> &vertices)
	{
		auto v = getVertexPool(vertexPool);
		v->rmElement(vertices);
		return (*this);
	}

	RenderManager &RenderManager::rmIndices(Key<IndexPool> const &indexPool, Key<Indices> &indices)
	{
		auto i = getIndexPool(indexPool);
		i->rmElement(indices);
		return (*this);
	}

	RenderManager &RenderManager::draw(GLenum mode, Key<Vertices> const &vertices, Key<VertexPool> const &pool)
	{
		auto p = getVertexPool(pool);
		p->startContext();
		p->bind();
		p->syncronisation();
		p->draw(mode, vertices);
	p->endContext();
	return (*this);
}

RenderManager &RenderManager::draw(GLenum mode, Key<Indices> const &indices, Key<Vertices> const &vertices, Key<IndexPool> const &indexPool, Key<VertexPool> const &vertexPool)
{
	auto pv = getVertexPool(vertexPool);
	auto pi = getIndexPool(indexPool);
	pv->startContext(*pi);
	pv->syncronisation();
	pi->syncronisation();
	pv->draw(mode, indices, vertices);
	pv->endContext();
	return (*this);
}
