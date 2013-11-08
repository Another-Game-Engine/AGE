#include "Renderer.hh"

#include "Core/Engine.hh"

#include "glm/gtc/type_ptr.hpp"

Renderer::Renderer(void)
{
}


Renderer::~Renderer(void)
{
	uninit();
}

bool Renderer::init()
{
	std::vector<std::string> list; list.push_back("layer0"); list.push_back("layer1"); list.push_back("layer2"); list.push_back("layer3");

	GameEngine::instance()->renderer().addShader("depthOnly", "../GameEngine/Shaders/depthOnly.vp", "../GameEngine/Shaders/depthOnly.fp");
	GameEngine::instance()->renderer().bindShaderToUniform("depthOnly", "PerFrame", "PerFrame");
	GameEngine::instance()->renderer().bindShaderToUniform("depthOnly", "PerModel", "PerModel");
	GameEngine::instance()->renderer().getShader("depthOnly")->addTarget(GL_COLOR_ATTACHMENT0).buildTargets();


	return _fbo.init(1920, 1080, list);
}

void		Renderer::addToRenderQueue(Components::MeshRenderer *obj)
{
	queueIt		it;

	if ((it = _queues.find(obj->getShader())) != std::end(_queues))
	{
		obj->setNext(it->second);
		it->second = obj;
	}
	else
	{
		obj->setNext(NULL);
		_queues[obj->getShader()] = obj;
	}
}

OpenGLTools::Shader		&Renderer::addShader(std::string const &name,
											 std::string const &vp,
											 std::string const &fp,
											 std::string const &geo)
{
	std::map<std::string, OpenGLTools::Shader*>::iterator	it;
   	OpenGLTools::Shader* shader;

	if ((it = _shaders.find(name)) == _shaders.end())
	{
		shader = new OpenGLTools::Shader;
		shader->init(vp, fp, geo);
		_shaders[name] = shader;
	}
	else
	{
		it->second->init(vp, fp, geo);
		shader = it->second;
	}
	return *shader;
}

bool		Renderer::removeShader(std::string const &name)
{
	std::map<std::string, OpenGLTools::Shader*>::iterator	it;

	if ((it = _shaders.find(name)) == _shaders.end())
		return (false);
	_shaders.erase(it);
	return (true);
}

OpenGLTools::Shader		*Renderer::getShader(std::string const &name)
{
	std::map<std::string, OpenGLTools::Shader*>::iterator	it;

	if ((it = _shaders.find(name)) == _shaders.end())
		return (NULL);
	return (it->second);
}

OpenGLTools::UniformBuffer	&Renderer::addUniform(std::string const &name)
{
	static GLuint					idx = 0;
	OpenGLTools::UniformBuffer		*buff = new OpenGLTools::UniformBuffer(idx++);

	_uniforms[name] = buff;
	return (*buff);
}

bool		Renderer::removeUniform(std::string const &name)
{
	std::map<std::string, OpenGLTools::UniformBuffer*>::iterator	it;

	if ((it = _uniforms.find(name)) == _uniforms.end())
		return (false);
	delete it->second;
	_uniforms.erase(it);
	return (true);
}

OpenGLTools::UniformBuffer	*Renderer::getUniform(std::string const &name)
{
	std::map<std::string, OpenGLTools::UniformBuffer*>::iterator	it;

	if ((it = _uniforms.find(name)) == _uniforms.end())
		return (NULL);
	return (it->second);
}

bool		Renderer::bindShaderToUniform(std::string const &shader,
										std::string const &blockName,
										std::string const &uniform)
{
	std::map<std::string, OpenGLTools::Shader*>::iterator			sh;
	std::map<std::string, OpenGLTools::UniformBuffer*>::iterator	un;

	if ((sh = _shaders.find(shader)) == _shaders.end() ||
		(un = _uniforms.find(uniform)) == _uniforms.end())
		return (false);
	sh->second->bindUniformBlock(blockName, *un->second);
	return (true);
}

void		Renderer::render()
{
	static double time = 0;
	Engine	&e = *GameEngine::instance();
	unsigned int textureOffset;

	time += e.timer().getElapsed();
	// Set les uniforms du block PerFrame
	e.renderer().getUniform("PerFrame")->setUniform("projection", e.getCurrentScene()->getCamera()->getProjection());
	e.renderer().getUniform("PerFrame")->setUniform("view", e.getCurrentScene()->getCamera()->getTransform());
	e.renderer().getUniform("PerFrame")->setUniform("time", (float)time);
	e.renderer().getUniform("PerFrame")->flushChanges();

	_fbo.renderBegin();
	_fbo.applyViewport();
	_fbo.clearColor();
	_fbo.clearZ();

	//render skybox before z-pass

	GameEngine::instance()->getCurrentScene()->getCamera()->update();

	// Depth pre-pass
	// to uncomment when depthOnly shader fixed

	//_fbo.zPassBegin();
	//unsigned int shaderIndex = 0;
	//OpenGLTools::Shader *shader = getShader("depthOnly");
	//shader->use();
	//_fbo.bindDrawTargets(shader->getTargets(), shader->getTargetsNumber());
	//unsigned int offset = 0;
	//for (auto &material : _materialManager.getMaterialList())
	//{
	//	for (auto &obj : material.second->getObjects())
	//	{
	//		getUniform("PerModel")->setUniform("model", obj->getFather()->getGlobalTransform());
	//		getUniform("PerModel")->flushChanges();
	//		obj->getMesh()->draw();
	//	}
	//}
	//_fbo.unbind();
	//_fbo.zPassEnd();


	// temporary z-pass
	// to erase when depthOnly shader fixed

	_fbo.zPassBegin();
	for (auto &material : _materialManager.getMaterialList())
	{
		unsigned int shaderIndex = 0;
		for (auto &shaderName : material.second->getShaders())
		{
			OpenGLTools::Shader *shader = getShader(shaderName);

			_fbo.bindDrawTargets(shader->getTargets(), shader->getTargetsNumber());

			shader->use();
			for (auto &obj : material.second->getObjects())
			{
				getUniform("PerModel")->setUniform("model", obj->getFather()->getGlobalTransform());
				getUniform("PerModel")->flushChanges();
				obj->getMesh()->draw();
			}
			_fbo.unbind();
		}
	}
	_fbo.zPassEnd();

	// Render pass

	for (auto &material : _materialManager.getMaterialList())
	{
		unsigned int shaderIndex = 0;
		for (auto &shaderName : material.second->getShaders())
		{
			OpenGLTools::Shader *shader = getShader(shaderName);

			_fbo.bindDrawTargets(shader->getTargets(), shader->getTargetsNumber());

			shader->use();
			unsigned int offset = _fbo.bind(shader);
			for (auto &obj : material.second->getObjects())
			{
				getUniform("PerModel")->setUniform("model", obj->getFather()->getGlobalTransform());
				getUniform("PerModel")->flushChanges();
				obj->bindTextures(offset, shader);
				obj->getMesh()->draw();
				obj->unbindTextures();
			}
			_fbo.unbind();
		}
	}
	_fbo.bindDrawTargets(nullptr, 0);
	_fbo.renderEnd();
	_fbo.debugRendering(getShader("fboToScreen"));
}


OpenGLTools::Framebuffer        &Renderer::getFbo()
{
	return _fbo;
}

void Renderer::uninit()
{
	for (shadersIt it = _shaders.begin(); it != _shaders.end(); ++it)
		delete it->second;
	for (uniformsIt it = _uniforms.begin(); it != _uniforms.end(); ++it)
		delete it->second;
	_shaders.clear();
	_uniforms.clear();
}