#ifndef			RENDERER_HH_
#define			RENDERER_HH_

#include <map>

#include "Entities/EntityData.hh"
#include "Components/MeshRenderer.hh"
#include "OpenGL/Shader.hh"
#include "OpenGL/UniformBuffer.hh"
#include "OpenGL/Framebuffer.hh"
#include "MaterialManager.hh"
#include "Utils/Any.hpp"
#include <core/Engine.hh>

class Renderer
{
private:
	typedef std::map<std::string, OpenGLTools::Shader*>::iterator			shadersIt;
	typedef std::map<std::string, OpenGLTools::UniformBuffer*>::iterator	uniformsIt;
	typedef std::map<std::string, Component::MeshRenderer*>::iterator		queueIt;
	typedef std::multimap<unsigned int, std::string>                        postEffectCol;
	typedef std::multimap<unsigned int, std::string>::iterator              postEffectColIt;

	OpenGLTools::Framebuffer                        _fbo;
	std::map<std::string,
		OpenGLTools::Shader*>						_shaders;
	std::map<std::string,
		OpenGLTools::UniformBuffer*>				_uniforms;
	std::map<std::string,
			Component::MeshRenderer* >				_queues; // Queues sorted by materials
	MaterialManager                                 _materialManager;
	postEffectCol                                   _postEffects;
	Engine &_engine;
public:
	Renderer(Engine *engine);
	virtual ~Renderer(void);

	bool init();

	// Shaders
	OpenGLTools::Shader		&addShader(std::string const &name,
									   std::string const &vp,
									   std::string const &fp,
									   std::string const &geo = "");
	bool					removeShader(std::string const &name);
	OpenGLTools::Shader		*getShader(std::string const &name);
	// Uniform buffers
	OpenGLTools::UniformBuffer	&addUniform(std::string const &name);
	bool							removeUniform(std::string const &name);
	OpenGLTools::UniformBuffer	*getUniform(std::string const &name);

	// Bind shader to uniform
	bool							bindShaderToUniform(std::string const &shader,
														std::string const &blockName,
														std::string const &uniform);
	// Render queue
	void							render(double time);

	OpenGLTools::Framebuffer        &getFbo();
	void                            uninit();
	MaterialManager &getMaterialManager();
	void addPostEffect(const std::string &name, unsigned int priority);
	inline Engine &getEngine() { return _engine; }
private:
	Renderer(const Renderer &o);
	Renderer &operator=(const Renderer &o);
};

#endif