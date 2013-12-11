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
	GameEngine::instance()->renderer().addShader("depthOnly", "../GameEngine/Shaders/depthOnly.vp", "../GameEngine/Shaders/depthOnly.fp");
	GameEngine::instance()->renderer().bindShaderToUniform("depthOnly", "PerFrame", "PerFrame");
	GameEngine::instance()->renderer().bindShaderToUniform("depthOnly", "PerModel", "PerModel");
	GameEngine::instance()->renderer().getShader("depthOnly")->addTarget(GL_COLOR_ATTACHMENT0).build();


	return _fbo.init(1920, 1080, 4);
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

void		Renderer::render(double time)
{
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
	_fbo.uninit();
}

void Renderer::addPostEffect(const std::string &name, unsigned int priority)
{
	_postEffects.insert(std::make_pair(priority, name));
}

MaterialManager &Renderer::getMaterialManager()
{
	return _materialManager;
}