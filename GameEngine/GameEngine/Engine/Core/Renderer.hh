#ifndef			RENDERER_HH_
#define			RENDERER_HH_

#include <map>

#include "Entities/Entity.hh"
#include "Components/MeshRenderer.hh"
#include "OpenGL/Shader.hh"
#include "OpenGL/UniformBuffer.hh"
#include "OpenGL/Framebuffer.hh"

class Renderer
{
private:
	OpenGLTools::Framebuffer                        _fbo;
	std::map<std::string,
		OpenGLTools::Shader*>						_shaders;
	std::map<std::string,
		OpenGLTools::UniformBuffer*>				_uniforms;
	std::map<std::string,
			Components::MeshRenderer* >				_queues; // Queues sorted by materials

	typedef std::map<std::string, OpenGLTools::Shader*>::iterator			shadersIt;
	typedef std::map<std::string, OpenGLTools::UniformBuffer*>::iterator	uniformsIt;
	typedef std::map<std::string, Components::MeshRenderer*>::iterator		queueIt;

public:
	Renderer(void);
	~Renderer(void);

	bool init();

	// Add an object that contain a render component to the render queue queueIdx
	void					addToRenderQueue(Components::MeshRenderer *obj); // queueIdx between 0 and 3
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
	void							render();
<<<<<<< HEAD

	OpenGLTools::Framebuffer        &getFbo();
=======
	void                            uninit();
>>>>>>> master
};

#endif