#include <OpenGL/RenderPass.hh>
#include <cassert>
#include <string>
#include <OpenGL/Shader.hh>

# undef DEBUG_MESSAGE
# define DEBUG_MESSAGE(type, from, reason, return_type) \
	{	assert(0 && std::string(std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "].").c_str()); return return_type; }


namespace gl
{
	RenderPass::RenderPass()
		: _shader(NULL)
	{
	}

	RenderPass::RenderPass(Shader &shader)
		: RenderPass()
	{
		_shader = &shader;
	}

	RenderPass::~RenderPass()
	{
	}

	RenderPass::RenderPass(RenderPass const &copy)
		: _shader(copy._shader)
	{
	}

	RenderPass &RenderPass::operator=(RenderPass const &r)
	{
		if (this != &r)
		{
			_shader = r._shader;
		}
		return (*this);
	}

	RenderPass &RenderPass::setScissor(glm::ivec4 const &area)
	{
		glScissor(area[0], area[1], area[2], area[3]);
		return (*this);
	}

	RenderPass &RenderPass::setClearValue(glm::vec4 const &color, float depth, unsigned char stencil)
	{
		glClearColor(color[0], color[1], color[2], color[3]);
		glClearDepth(depth);
		glClearStencil(stencil);
		return (*this);
	}

	RenderPass &RenderPass::setColorMask(GLuint index, glm::bvec4 const &color)
	{
		glColorMaski(index, color[0], color[1], color[2], color[3]);
		return (*this);
	}

	RenderPass &RenderPass::setDepthMask(bool depth)
	{
		glDepthMask(depth);
		return (*this);
	}

	RenderPass &RenderPass::setStencilMask(uint8_t front, uint8_t back)
	{
		glStencilMaskSeparate(GL_FRONT, front);
		glStencilMaskSeparate(GL_BACK, back);
		return (*this);
	}

	RenderPass &RenderPass::clear(bool color, bool depth, bool stencil)
	{
		GLenum colorBit = color ? GL_COLOR_BUFFER_BIT : 0;
		GLenum depthBit = depth ? GL_DEPTH_BUFFER_BIT : 0;
		GLenum stencilBit = stencil ? GL_STENCIL_BUFFER_BIT : 0;
		glClear(colorBit | depthBit | stencilBit);
		return (*this);
	}

	RenderPass &RenderPass::setTest(bool scissor, bool stencil, bool depth)
	{
		GLint bits; glGetIntegerv(GL_STENCIL_BITS, &bits);
		if (bits < 8) DEBUG_MESSAGE("warning", "RenderPass - setTest", "stencil < 8", *this);
		if (scissor) { glEnable(GL_SCISSOR_TEST); }
		else { glDisable(GL_SCISSOR_TEST); }
		if (stencil) { glEnable(GL_STENCIL_TEST); }
		else { glDisable(GL_STENCIL_TEST); }
		if (depth) { glEnable(GL_DEPTH_TEST); }
		else { glDisable(GL_DEPTH_TEST); }
		return (*this);
	}

	RenderPass &RenderPass::setStencilFunctionFrontFace(GLenum func, int ref, uint8_t mask)
	{
		glStencilFuncSeparate(GL_FRONT, func, ref, mask);
		return (*this);
	}

	RenderPass &RenderPass::setStencilOperationFrontFace(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		glStencilOpSeparate(GL_FRONT, opStencilFail, opDepthFail, opDepthPass);
		return (*this);
	}

	RenderPass &RenderPass::setStencilFunctionBackFace(GLenum func, int ref, uint8_t mask)
	{
		glStencilFuncSeparate(GL_BACK, func, ref, mask);
		return (*this);
	}

	RenderPass &RenderPass::setStencilOperationBackFace(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		glStencilOpSeparate(GL_BACK, opStencilFail, opDepthFail, opDepthPass);
		return (*this);
	}

	RenderPass &RenderPass::setStencilFunction(GLenum func, int ref, uint8_t mask)
	{
		glStencilFunc(func, ref, mask);
		return (*this);
	}

	RenderPass &RenderPass::setStencilOperation(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		glStencilOp(opStencilFail, opDepthFail, opDepthPass);
		return (*this);
	}
	
	RenderPass &RenderPass::use()
	{
		if (!_shader)
			DEBUG_MESSAGE("Warning", "RenderPass - use", "no shader assign on this renderPass", *this);
		_shader->use();
		return (*this);
	}

	RenderPass &RenderPass::setBlend(int drawBuffer, bool state)
	{
		if (state)
			glEnablei(GL_BLEND, drawBuffer);
		else
			glDisablei(GL_BLEND, drawBuffer);
		return (*this);
	}

	RenderPass &RenderPass::setBlendEquation(GLenum colorMode, GLenum alphaMode)
	{
		glBlendEquationSeparate(colorMode, alphaMode);
		return (*this);
	}

	RenderPass &RenderPass::setBlendEquation(GLenum mode)
	{
		glBlendEquation(mode);
		return (*this);
	}

	RenderPass &RenderPass::setBlendFunc(GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha)
	{
		glBlendFuncSeparate(srcRGB, destRGB, srcAlpha, destRGB);
		return (*this);
	}
	
	RenderPass &RenderPass::setBlendFunc(GLenum src, GLenum dest)
	{
		glBlendFunc(src, dest);
		return (*this);
	}

	RenderPass &RenderPass::setBlendConstant(glm::vec4 const &blendColor)
	{
		glBlendColor(blendColor[0], blendColor[1], blendColor[2], blendColor[4]);
		return (*this);
	}

}