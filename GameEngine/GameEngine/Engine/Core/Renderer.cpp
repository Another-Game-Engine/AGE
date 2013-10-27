#include "Renderer.hh"

#include "Core/Engine.hh"

#include "glm/gtc/type_ptr.hpp"

Renderer::Renderer(void)
{
}


Renderer::~Renderer(void)
{
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

OpenGLTools::Shader		&Renderer::addShader(std::string const &name, std::string const &vp, std::string const &fp)
{
	std::map<std::string, OpenGLTools::Shader*>::iterator	it;
   	OpenGLTools::Shader* shader;

	if ((it = _shaders.find(name)) == _shaders.end())
	{
		shader = new OpenGLTools::Shader;
		shader->init(vp, fp);
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

	time += e.timer().getElapsed();
	// Set les uniforms du block PerFrame
	e.renderer().getUniform("PerFrame")->setUniform("projection", e.getCurrentScene()->getCamera()->getProjection());
	e.renderer().getUniform("PerFrame")->setUniform("view", e.getCurrentScene()->getCamera()->getTransform());
	e.renderer().getUniform("PerFrame")->setUniform("time", (float)time);
	e.renderer().getUniform("PerFrame")->flushChanges();

	GameEngine::instance()->getCurrentScene()->getCamera()->update();

	queueIt			it;

    for (it = std::begin(_queues);  it != std::end(_queues);  ++it)
    {
		OpenGLTools::Shader		*currentShader = getShader(it->first);
		// Set the uniforms values for the shader
		assert(currentShader != NULL && "Shader binded that does not exist");
		currentShader->use();

		Components::MeshRenderer		*cur = it->second;

		while (cur)
		{
			// Set les uniforms du block PerModel
			GameEngine::instance()->renderer().getUniform("PerModel")->setUniform("model", cur->getFather()->getGlobalTransform());
			GameEngine::instance()->renderer().getUniform("PerModel")->flushChanges();

			cur->bindTextures();
			cur->getMesh()->draw();
			cur->unbindTextures();
			cur = cur->getNext();
        }
    }
	_queues.clear();
}
