#pragma once

#include <Utils/OpenGL.hh>
#include <glm/glm.hpp>
#include <stdint.h>
#include <tuple>

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
		RenderPass &setDepthMask(bool depth);
		RenderPass &setStencilMask(uint8_t front, uint8_t back);
		RenderPass &clear(bool color, bool depth, bool stencil);
		RenderPass &setScissor(glm::ivec4 const &area);
		RenderPass &setStencilFunctionFrontFace(GLenum func, int ref, uint8_t mask);
		RenderPass &setStencilOperationFrontFace(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
		RenderPass &setStencilFunctionBackFace(GLenum func, int ref, uint8_t mask);
		RenderPass &setStencilOperationBackFace(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
		RenderPass &setStencilFunction(GLenum func, int ref, uint8_t mask);
		RenderPass &setStencilOperation(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
		RenderPass &setBlend(int drawBuffer, bool state);
		RenderPass &setBlendEquation(GLenum colorMode, GLenum alphaMode);
		RenderPass &setBlendFunc(GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha);

		RenderPass &setTest(bool scissor, bool stencil, bool depth);
		RenderPass &use();

	private:
		Shader *_shader;

	};
}