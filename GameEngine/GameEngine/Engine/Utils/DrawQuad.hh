#pragma once

#include <OpenGL\Shader.hh>
#include <OpenGL\VertexManager.hh>
#include <Core\Engine.hh>
#include <Core\Renderer.hh>

class DrawQuad
{
public:
	DrawQuad();
	~DrawQuad();

	void	init(Engine &engine);
	void	draw(GLuint texture, int sampleNbr, glm::uvec2 const &textureSize);

private:
	void						initShaders();

	VertexManager<2>			*_vertexManager;
	Vertice<2>					_quad;
	OpenGLTools::Shader			*_fboToScreen;
	OpenGLTools::Shader			*_fboToScreenMultisampled;
	std::shared_ptr<Renderer>	_renderer;
};

