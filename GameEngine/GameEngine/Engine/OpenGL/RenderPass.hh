#pragma once

#include <Utils/OpenGL.hh>
#include <glm/glm.hpp>
#include <stdint.h>
#include <tuple>
#include <utility>
#include <vector>
#include <functional>

namespace gl
{
	class Shader;
	class Drawable;

	// set buffer config
	//void setClearValue(glm::vec4 const &color, float depth, uint8_t stencil);
	//void setColorMask(GLuint index, glm::bvec4 const &color);
	//void setDepthMask(bool depth);
	//void setStencilMask(uint8_t front, uint8_t back);
	//void clear(bool color, bool depth, bool stencil);
	//void setScissor(glm::ivec4 const &area);
	//void setStencilFunctionFrontFace(GLenum func, int ref, uint8_t mask);
	//void setStencilOperationFrontFace(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
	//void setStencilFunctionBackFace(GLenum func, int ref, uint8_t mask);
	//void setStencilOperationBackFace(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
	//void setStencilFunction(GLenum func, int ref, uint8_t mask);
	//void setStencilOperation(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
	//void setBlend(int drawBuffer, bool state);
	//void setBlendEquation(GLenum colorMode, GLenum alphaMode);
	//void setBlendEquation(GLenum mode);
	//void setBlendFunc(GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha);
	//void setBlendFunc(GLenum src, GLenum dest);
	//void setBlendConstant(glm::vec4 const &blendColor);
	//void setTest(bool scissor, bool stencil, bool depth);

	struct Task
	{
		int nbrParams;
		void **params;
		void(*func)(void **);
	};

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

		RenderPass &draw(Drawable *objectRender, size_t nbrObjectRender);
		RenderPass &pushSetScissorTask(glm::ivec4 const &area);

	private:
		Shader *_shader;
		std::vector<Task> _tasks;
	};

}