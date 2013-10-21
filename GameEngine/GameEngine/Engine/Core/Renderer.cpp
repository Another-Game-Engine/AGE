#include "Renderer.hh"


Renderer::Renderer(void)
{
}


Renderer::~Renderer(void)
{
}

void		Renderer::addToRenderQueue(SmartPointer<Components::MeshRenderer> const &obj)
{
	std::string		name = obj->getShader();
	std::map<std::string, SmartPointer<Components::MeshRenderer> >::iterator	it;

	if ((it = _queues.find(name)) == _queues.end())
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

	if ((it = _shaders.find(name)) == _shaders.end())
		_shaders[name] = new OpenGLTools::Shader;
	else
		it->second->init(vp, fp);
	return (*it->second);
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
	std::map<std::string, SmartPointer<Components::MeshRenderer> >::iterator	it;

	it = _queues.begin();
	while (it != _queues.end()) // for each shader
	{
		OpenGLTools::Shader		*currentShader = getShader(it->first);
		SmartPointer<Components::MeshRenderer>	obj = it->second;

		// Set the uniforms values for the shader
		assert(currentShader != NULL && "Shader binded that does not exist");
		currentShader->use();
		while (obj != SmartPointer<Components::MeshRenderer>(NULL)) // for each object
		{
			obj->getMesh()->draw();
			obj = obj->getNext();
		}
		++it;
	}
}
