#ifndef RENDERER_H_
# define RENDERER_H_

# include <vector>
#include <map>
#include "Entities/EntityData.hh"
#include "Components/MeshRenderer.hh"
#include "OpenGL/Shader.hh"
#include "OpenGL/UniformBuffer.hh"
#include "OpenGL/Framebuffer.hh"
#include "Utils/Any.hpp"
#include <core/Engine.hh>
#include <Utils/Dependency.hpp>
#include <memory>

/*class Renderer : public Dependency
{
private:
	std::vector<OpenGLTools::Shader> _shaders;
	std::vector<OpenGLTools::Pipeline> _pipelines;
	std::vector<std::unique<OpenGLTools::ITexture>> _texture;

public:
	Renderer();
	~Renderer();
	void addShader(OpenGLTools::Shader &&shader);
	void deleteShader(OpenGLTools::Shader &&shader);
	void addPipeline(std::string &&pipelineName, std::string &&shader);
	void deletePipeline(std::string &&pipelineName);
	Pipeline getPipeline(std::string &&pipelineName);
	template <TYPE> void addTexture(std::string &&textureName, Texture<TYPE> texture);
	void deleteTexture(std::string &&textureName);
};*/

class Renderer : public Dependency
{
private:
	OpenGLTools::Framebuffer                        _fbo;
	std::map<std::string,
		std::shared_ptr<OpenGLTools::Shader>>		_shaders;
	std::map<std::string,
		std::shared_ptr<OpenGLTools::UniformBuffer>> _uniforms;
	std::multimap<unsigned int, std::string>         _postEffects;
public:
	Renderer();
	virtual ~Renderer(void);

	bool init();

	// Shaders
	std::shared_ptr<OpenGLTools::Shader>		addShader(
		std::string const &name,
		std::string const &vp,
		std::string const &fp,
		std::string const &geo = "");

	bool                    					removeShader(std::string const &name);
	std::shared_ptr<OpenGLTools::Shader>		getShader(std::string const &name) const;
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

#endif /*!RENDERER_H_*/