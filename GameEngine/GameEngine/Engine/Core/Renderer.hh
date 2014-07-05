#pragma once

#include <map>

#include "Components/MeshRenderer.hh"
#include "OpenGL/Shader.hh"
#include "OpenGL/UniformBuffer.hh"
#include "OpenGL/Framebuffer.hh"
#include "Utils/Any.hpp"
#include <core/Engine.hh>
#include <Utils/Dependency.hpp>
#include <memory>

class Renderer : public Dependency<Renderer>
{
private:
	OpenGLTools::Framebuffer                        _fbo;
	std::map<std::string,
		std::shared_ptr<gl::Shader>>		_shaders;
	std::map<std::string,
		std::shared_ptr<OpenGLTools::UniformBuffer>> _uniforms;
	std::multimap<unsigned int, std::string>         _postEffects;
public:
	Renderer();
	virtual ~Renderer(void);

	bool init();

	// Shaders
	std::shared_ptr<gl::Shader>		addShader(
		std::string const &name,
		std::string const &vp,
		std::string const &fp,
		std::string const &geo = "");

	bool                    					removeShader(std::string const &name);
	std::shared_ptr<gl::Shader>		getShader(std::string const &name) const;
	// Uniform buffers
	std::shared_ptr<OpenGLTools::UniformBuffer>	addUniform(std::string const &name);
	bool            							removeUniform(std::string const &name);
	std::shared_ptr<OpenGLTools::UniformBuffer>	getUniform(std::string const &name) const;

	// Bind shader to uniform
	bool							bindShaderToUniform(std::string const &shader,
														std::string const &blockName,
														std::string const &uniform);
	// Render queue
	void							render(double time);

	OpenGLTools::Framebuffer        &getFbo();
	void                            uninit();
	void addPostEffect(const std::string &name, unsigned int priority);
private:
	Renderer(const Renderer &o);
	Renderer &operator=(const Renderer &o);
};
