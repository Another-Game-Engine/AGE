#ifndef			RENDERER_HH_
#define			RENDERER_HH_

#include <map>

#include "Entities/Entity.hh"
#include "Components/MeshRenderer.hh"
#include "OpenGL/Shader.hh"
#include "OpenGL/UniformBuffer.hh"

class Renderer
{
private:
	std::map<std::string,
		OpenGLTools::Shader*>						_shaders;
	std::map<std::string,
		OpenGLTools::UniformBuffer<>*>				_uniforms;
	std::multimap<std::string,
		Components::MeshRenderer* >	_queues; // Queues sorted by materials

typedef std::multimap<std::string,
		Components::MeshRenderer* > queueType;
typedef queueType::iterator queueIt;

public:
	Renderer(void);
	~Renderer(void);

	// Add an object that contain a render component to the render queue queueIdx
	void					addToRenderQueue(Components::MeshRenderer *obj); // queueIdx between 0 and 3
	// Shaders
	OpenGLTools::Shader		&addShader(std::string const &name, std::string const &vp, std::string const &fp);
	bool					removeShader(std::string const &name);
	OpenGLTools::Shader		*getShader(std::string const &name);
	// Uniform buffers
	OpenGLTools::UniformBuffer<>	&addUniform(std::string const &name);
	bool							removeUniform(std::string const &name);
	OpenGLTools::UniformBuffer<>	*getUniform(std::string const &name);

	// Bind shader to uniform
	bool							bindShaderToUniform(std::string const &shader,
														std::string const &blockName,
														std::string const &uniform);
	// Render queue
	void							render();
};

#endif