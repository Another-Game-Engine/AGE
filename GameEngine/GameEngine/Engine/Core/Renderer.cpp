#include "Renderer.hh"

#include "Core/Engine.hh"

Renderer::Renderer(void)
{
}


Renderer::~Renderer(void)
{
}

void		Renderer::addToRenderQueue(SmartPointer<Components::MeshRenderer> const &obj)
{
	std::string		name = obj->getShader();
	auto &it = _queues.find(name);

	if (it == std::end(_queues))
	{
		obj->setNext(NULL);
		_queues[obj->getShader()] = obj;
	}
	else
	{
		obj->setNext(it->second);
		it->second = obj;
	}
}

OpenGLTools::Shader		&Renderer::addShader(std::string const &name, std::string const &vp, std::string const &fp)
{
	std::map<std::string, OpenGLTools::Shader*>::iterator	it;
   	OpenGLTools::Shader* shader;

	if ((it = _shaders.find(name)) == _shaders.end())
	{
		shader = new OpenGLTools::Shader;
		_shaders[name] = shader;
	}
	else
	{
		it->second->init(vp, fp);
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

OpenGLTools::UniformBuffer<>	&Renderer::addUniform(std::string const &name)
{
	static GLuint					idx = 0;
	OpenGLTools::UniformBuffer<>	*buff = new OpenGLTools::UniformBuffer<>(idx++);

	buff->init();
	_uniforms[name] = buff;
	return (*buff);
}

bool		Renderer::removeUniform(std::string const &name)
{
	std::map<std::string, OpenGLTools::UniformBuffer<>*>::iterator	it;

	if ((it = _uniforms.find(name)) == _uniforms.end())
		return (false);
	delete it->second;
	_uniforms.erase(it);
	return (true);
}

OpenGLTools::UniformBuffer<>	*Renderer::getUniform(std::string const &name)
{
	std::map<std::string, OpenGLTools::UniformBuffer<>*>::iterator	it;

	if ((it = _uniforms.find(name)) == _uniforms.end())
		return (NULL);
	return (it->second);
}

bool		Renderer::bindShaderToUniform(std::string const &shader,
										std::string const &blockName,
										std::string const &uniform)
{
	std::map<std::string, OpenGLTools::Shader*>::iterator			sh;
	std::map<std::string, OpenGLTools::UniformBuffer<>*>::iterator	un;

	if ((sh = _shaders.find(shader)) == _shaders.end() ||
		(un = _uniforms.find(uniform)) == _uniforms.end())
		return (false);
	sh->second->bindUniformBlock(blockName, *un->second);
	return (true);
}

void		Renderer::render()
{
	// Set les uniforms du block PerFrame
	float		light[] = {0.0f, 5.0f, 0.0f};
	void		*data;

	data = (void*)&GameEngine::instance()->getCurrentScene()->getCamera()->getProjection();
	GameEngine::instance()->renderer().getUniform("PerFrame")->setUniform("vProjection", data);
	data = (void*)&GameEngine::instance()->getCurrentScene()->getCamera()->getTransform();
	GameEngine::instance()->renderer().getUniform("PerFrame")->setUniform("vView", data);
	GameEngine::instance()->renderer().getUniform("PerFrame")->setUniform("fLightSpot", light);
	GameEngine::instance()->renderer().getUniform("PerFrame")->flushChanges();

	auto &it = std::begin(_queues);
	while (it != std::end(_queues)) // for each shader
	{
		OpenGLTools::Shader		*currentShader = getShader(it->first);
		SmartPointer<Components::MeshRenderer>	obj = it->second;

		// Set the uniforms values for the shader
		assert(currentShader != NULL && "Shader binded that does not exist");
		currentShader->use();
		while (obj != SmartPointer<Components::MeshRenderer>(NULL)) // for each object
		{
			// Set les uniforms du block PerModel
			data = (void*)&obj->getFather()->getGlobalTransform();
			GameEngine::instance()->renderer().getUniform("PerModel")->setUniform("vModel", data);
			GameEngine::instance()->renderer().getUniform("PerModel")->flushChanges();
			
			obj->getMesh()->draw();
			obj = obj->getNext();
		}
		++it;
	}
	_queues.clear();
}
