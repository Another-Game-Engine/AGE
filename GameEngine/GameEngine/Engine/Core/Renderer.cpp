#include "Renderer.hh"

#include "Core/Engine.hh"

#include "glm/gtc/type_ptr.hpp"

Renderer::Renderer()
{
}


Renderer::~Renderer(void)
{
	uninit();
}

bool Renderer::init()
{
	addShader("depthOnly", "../../Shaders/depthOnly.vp", "../../Shaders/depthOnly.fp");
	bindShaderToUniform("depthOnly", "PerFrame", "PerFrame");
	bindShaderToUniform("depthOnly", "PerModel", "PerModel");


	//return _fbo.init(1920, 1080, 4);
	return true;
}

std::shared_ptr<OpenGLTools::Shader>		Renderer::addShader(std::string const &name,
											 std::string const &vp,
											 std::string const &fp,
											 std::string const &geo)
{
	auto it = _shaders.find(name);
	std::shared_ptr<OpenGLTools::Shader> shader{nullptr};

	if (it == std::end(_shaders))
	{
		if (geo == std::string(""))
			shader = std::make_shared<OpenGLTools::Shader>(std::move(std::string(vp)), std::move(std::string(fp)));
		else
			shader = std::make_shared<OpenGLTools::Shader>(std::move(std::string(vp)), std::move(std::string(fp)), std::move(std::string(geo)));
		_shaders[name] = shader;
	}
	else
	{
		if (geo == std::string(""))
			it->second = std::make_shared<OpenGLTools::Shader>(std::move(std::string(vp)), std::move(std::string(fp)));
		else
			it->second = std::make_shared<OpenGLTools::Shader>(std::move(std::string(vp)), std::move(std::string(fp)), std::move(std::string(geo)));
		shader = it->second;
	}
	return shader;
}

bool		Renderer::removeShader(std::string const &name)
{
	auto it = _shaders.find(name);

	if (it == std::end(_shaders))
		return (false);
	_shaders.erase(it);
	return (true);
}

std::shared_ptr<OpenGLTools::Shader>		Renderer::getShader(std::string const &name) const
{
	auto it = _shaders.find(name);

	if (it == std::end(_shaders))
		return (nullptr);
	return (it->second);
}

std::shared_ptr<OpenGLTools::UniformBuffer>	Renderer::addUniform(std::string const &name)
{
	static GLuint idx = 0;
	auto it = _uniforms.find(name);
	if (it != std::end(_uniforms))
		return it->second;
	auto buff = std::make_shared<OpenGLTools::UniformBuffer>(idx++);
	_uniforms[name] = buff;
	return (buff);
}

bool		Renderer::removeUniform(std::string const &name)
{
	auto it = _uniforms.find(name);

	if (it == std::end(_uniforms))
		return (false);
	_uniforms.erase(it);
	return (true);
}

std::shared_ptr<OpenGLTools::UniformBuffer>	Renderer::getUniform(std::string const &name) const
{
	auto it = _uniforms.find(name);

	if (it == std::end(_uniforms))
		return (nullptr);
	return (it->second);
}

bool		Renderer::bindShaderToUniform(std::string const &shader,
										std::string const &blockName,
										std::string const &uniform)
{
	auto sh = _shaders.find(shader);
	auto un = _uniforms.find(uniform);

	if (sh == std::end(_shaders) ||
		un == std::end(_uniforms))
		return (false);
	//sh->second->bindUniformBlock(blockName, un->second);
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
	_shaders.clear();
	_uniforms.clear();
	//_fbo.uninit();
}

void Renderer::addPostEffect(const std::string &name, unsigned int priority)
{
	_postEffects.insert(std::make_pair(priority, name));
}