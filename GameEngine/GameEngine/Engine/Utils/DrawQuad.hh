#pragma once

#include <OpenGL\Shader.hh>
#include <OpenGL\VertexManager.hh>
#include <Core\Engine.hh>
#include <Core\Renderer.hh>
#include <Utils/DependenciesInjector.hpp>

class DrawQuad
{
public:
	DrawQuad();
	~DrawQuad();

	void	init(std::weak_ptr<DependenciesInjector> &&engine);
	void	draw(GLuint texture, int sampleNbr, glm::uvec2 const &textureSize);

private:
	void						initShaders();

	VertexManager<2>			*_vertexManager;
	Vertice<2>					_quad;
	std::shared_ptr<OpenGLTools::Shader>			_fboToScreen;
	std::shared_ptr<OpenGLTools::Shader>			_fboToScreenMultisampled;
	std::shared_ptr<Renderer>	_renderer;
};

