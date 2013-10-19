#ifndef			RENDERER_HH_
#define			RENDERER_HH_

#include <map>

#include "Entities/AEntity.hh"

#include "OpenGL/Shader.hh"
#include "OpenGL/UniformBuffer.hh"

class Renderer
{
private:
	std::map<std::string,
		OpenGLTools::Shader*>					_shaders;
	std::map<std::string,
		OpenGLTools::UniformBuffer<>*>			_uniforms;
	std::map<std::string,
		SmartPointer<MeshRenderer> >			_queues[4]; // Queues sorted by materials

public:
	Renderer(void);
	~Renderer(void);

	// Add an object that contain a render component to the render queue queueIdx
	bool		addToRenderQueue(SmartPointer<AEntity> object, unsigned int	queueIdx); // queueIdx between 0 and 3
	// Shaders
	bool		addShader(std::string const &name, std::string const &vp, std::string const &fp);
	bool		removeShader(std::string const &name);
	bool		shaderExist(std::string const &name);
	// Uniform buffers
	bool		addUniform(std::string const &name,
						   std::string const &blockName,
						   std::string const *varNames,
						   size_t varNbr);
	bool		removeUniform(std::string const &name);
	// Bind shader to uniform
	bool		bindShaderToUniform(std::string const &shader,
									std::string const &blockName,
									std::string const &uniform);
};

#endif