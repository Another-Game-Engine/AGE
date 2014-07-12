#include <OpenGL/RenderPass.hh>
#include <cassert>
#include <string>
#include <OpenGL/Shader.hh>
#include <OpenGL/Drawable.hh>

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

	void setScissor(glm::ivec4 const &area)
	{
		glScissor(area[0], area[1], area[2], area[3]);
	}

	void setClearValue(glm::vec4 const &color, float depth, unsigned char stencil)
	{
		glClearColor(color[0], color[1], color[2], color[3]);
		glClearDepth(depth);
		glClearStencil(stencil);
	}

	void setColorMask(GLuint index, glm::bvec4 const &color)
	{
		glColorMaski(index, color[0], color[1], color[2], color[3]);
	}

	void setDepthMask(bool depth)
	{
		glDepthMask(depth);
	}

	void setStencilMask(uint8_t front, uint8_t back)
	{
		glStencilMaskSeparate(GL_FRONT, front);
		glStencilMaskSeparate(GL_BACK, back);
	}

	void clear(bool color, bool depth, bool stencil)
	{
		GLenum colorBit = color ? GL_COLOR_BUFFER_BIT : 0;
		GLenum depthBit = depth ? GL_DEPTH_BUFFER_BIT : 0;
		GLenum stencilBit = stencil ? GL_STENCIL_BUFFER_BIT : 0;
		glClear(colorBit | depthBit | stencilBit);
	}

	void setTest(bool scissor, bool stencil, bool depth)
	{
		GLint bits; glGetIntegerv(GL_STENCIL_BITS, &bits);
		if (bits < 8) DEBUG_MESSAGE("warning", "RenderPass - setTest", "stencil < 8", );
		if (scissor) { glEnable(GL_SCISSOR_TEST); }
		else { glDisable(GL_SCISSOR_TEST); }
		if (stencil) { glEnable(GL_STENCIL_TEST); }
		else { glDisable(GL_STENCIL_TEST); }
		if (depth) { glEnable(GL_DEPTH_TEST); }
		else { glDisable(GL_DEPTH_TEST); }
	}

	void setStencilFunctionFrontFace(GLenum func, int ref, uint8_t mask)
	{
		glStencilFuncSeparate(GL_FRONT, func, ref, mask);
	}

	void setStencilOperationFrontFace(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		glStencilOpSeparate(GL_FRONT, opStencilFail, opDepthFail, opDepthPass);
	}

	void setStencilFunctionBackFace(GLenum func, int ref, uint8_t mask)
	{
		glStencilFuncSeparate(GL_BACK, func, ref, mask);
	}

	void setStencilOperationBackFace(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		glStencilOpSeparate(GL_BACK, opStencilFail, opDepthFail, opDepthPass);
	}

	void setStencilFunction(GLenum func, int ref, uint8_t mask)
	{
		glStencilFunc(func, ref, mask);
	}

	void setStencilOperation(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		glStencilOp(opStencilFail, opDepthFail, opDepthPass);
	}

	void setBlend(int drawBuffer, bool state)
	{
		if (state)
			glEnablei(GL_BLEND, drawBuffer);
		else
			glDisablei(GL_BLEND, drawBuffer);
	}

	void setBlendEquation(GLenum colorMode, GLenum alphaMode)
	{
		glBlendEquationSeparate(colorMode, alphaMode);
	}

	void setBlendEquation(GLenum mode)
	{
		glBlendEquation(mode);
	}

	void setBlendFunc(GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha)
	{
		glBlendFuncSeparate(srcRGB, destRGB, srcAlpha, destRGB);
	}

	void setBlendFunc(GLenum src, GLenum dest)
	{
		glBlendFunc(src, dest);
	}

	void setBlendConstant(glm::vec4 const &blendColor)
	{
		glBlendColor(blendColor[0], blendColor[1], blendColor[2], blendColor[4]);
	}

	RenderPass &RenderPass::pushSetScissorTask(glm::ivec4 const &area)
	{
		Command task;

		task.nbrParams = 1;
		task.params = new void *[task.nbrParams];
		task.params[0] = new glm::ivec4;
		memcpy(task.params[0], &area, sizeof(glm::ivec4));
		task.func = &setScissor;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::draw(Drawable *objectRender, size_t nbrObjectRender)
	{
		
		if (!_shader)
			DEBUG_MESSAGE("Warning", "RenderPass - use", "no shader assign on this renderPass", *this);
		_shader->use();
		return (*this);
	}
}