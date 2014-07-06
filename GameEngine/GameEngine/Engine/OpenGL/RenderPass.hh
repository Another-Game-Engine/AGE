#pragma once

#include <Utils/OpenGL.hh>
#include <glm/glm.hpp>
#include <stdint.h>

namespace gl
{
	class Shader;

	//!\file RenderPass.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class RenderPass
	//!\brief Handle one of opengl render pass
	class RenderPass
	{
	public:
		RenderPass();
		RenderPass(Shader &shader);
		~RenderPass();
		RenderPass(RenderPass const &copy);
		RenderPass &operator=(RenderPass const &r);

		RenderPass &setClearValue(glm::vec4 const &color, float depth, uint8_t stencil);
		RenderPass &setColorMask(GLuint index, glm::bvec4 const &color);
		RenderPass &setDepthStencilMask(bool depth, uint8_t front, uint8_t back);
		RenderPass &setClearOption(bool color, bool depth, bool stencil);
		RenderPass &setScissorArea(glm::ivec4 const &area);
		RenderPass &setTest(bool scissor, bool stencil, bool depth);
		RenderPass &use();

	private:
		Shader *_shader;
		// clean buffer
		GLbitfield _clearColor;
		GLbitfield _clearDepth;
		GLbitfield _clearStencil;
		
		// clear color
		glm::vec4 _color;
		float _depth;
		uint8_t _stencil;
		
		// mask
		glm::bvec4 _maskColor[GL_MAX_COLOR_ATTACHMENTS];
		bool _maskDepth;
		uint8_t _maskStencilBack;
		uint8_t _maskStencilFront;

		// scissor
		glm::ivec4 _area;

		// Test
		bool _scissorTest;
		bool _stencilTest;
		bool _depthTest;
		
	};
}