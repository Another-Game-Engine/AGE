#pragma once

# include <Utils/OpenGL.hh>
# include <glm/glm.hpp>
# include <memory>
# include <OpenGL/Vertice.hh>

namespace OpenGLTools
{
	class Shader;
}
class Renderer;
template <uint8_t SIZE_ATTRIBUTE> class VertexManager;

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

