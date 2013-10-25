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
	_queues.insert(std::make_pair(obj->getShader(), obj));
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

	data = (void*)glm::value_ptr(GameEngine::instance()->getCurrentScene()->getCamera()->getProjection());
	GameEngine::instance()->renderer().getUniform("PerFrame")->setUniform("vProjection", data);
	data = (void*)glm::value_ptr(GameEngine::instance()->getCurrentScene()->getCamera()->getTransform());
	GameEngine::instance()->renderer().getUniform("PerFrame")->setUniform("vView", data);
	GameEngine::instance()->renderer().getUniform("PerFrame")->setUniform("fLightSpot", light);
	GameEngine::instance()->renderer().getUniform("PerFrame")->flushChanges();

	queueIt mIt, sIt;

    for (mIt = std::begin(_queues);  mIt != std::end(_queues);  mIt = sIt)
    {
        std::string key = mIt->first;
        std::pair<queueIt, queueIt> keyRange = _queues.equal_range(key);

		OpenGLTools::Shader		*currentShader = getShader(key);
		// Set the uniforms values for the shader
		assert(currentShader != NULL && "Shader binded that does not exist");
		currentShader->use();

        for (sIt = keyRange.first;  sIt != keyRange.second;  ++sIt)
        {
			// Set les uniforms du block PerModel
			data = (void*)glm::value_ptr(sIt->second->getFather()->getGlobalTransform());
			GameEngine::instance()->renderer().getUniform("PerModel")->setUniform("vModel", data);
			GameEngine::instance()->renderer().getUniform("PerModel")->flushChanges();

			sIt->second->bindTextures();
			sIt->second->getMesh()->draw();
			sIt->second->unbindTextures();
        }
    }
	_queues.clear();
}
