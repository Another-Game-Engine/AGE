#include <OpenGL/BufferSettings.hh>
#include <Utils/OpenGL.hh>
#include <glm/glm.hpp>
#include <assert.h>
#include <memory>
#include <string>

# undef DEBUG_MESSAGE
# define DEBUG_MESSAGE(type, from, reason, return_type) \
	{	assert(0 && std::string(std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "].").c_str()); return return_type; }

#define CONVERT(type, index) (*((type *)data[index]))

namespace gl
{

	void setScissor(void **data)
	{
		glScissor((*(glm::ivec4 *)data)[0], (*(glm::ivec4 *)data)[1], (*(glm::ivec4 *)data)[2], (*(glm::ivec4 *)data)[3]);
	}

	void setClearValue(void **data)
	{
		glClearColor(CONVERT(glm::vec4, 0).r, CONVERT(glm::vec4, 0).g, CONVERT(glm::vec4, 0).b, CONVERT(glm::vec4, 0).a);
		glClearDepth(CONVERT(float, 1));
		glClearStencil(CONVERT(unsigned char, 2));
	}

	void setColorMask(void **data)
	{
		glColorMaski(CONVERT(GLuint, 0), CONVERT(glm::bvec4, 1)[0], CONVERT(glm::bvec4, 1)[1], CONVERT(glm::bvec4, 1)[2], CONVERT(glm::bvec4, 1)[3]);
	}

	void setDepthMask(void **data)
	{
		glDepthMask(CONVERT(bool, 0));
	}

	void setStencilMask(void **data)
	{
		glStencilMaskSeparate(GL_FRONT, CONVERT(uint8_t, 0));
		glStencilMaskSeparate(GL_BACK, CONVERT(uint8_t, 1));
	}

	void clear(bool color, bool depth, bool stencil)
	{
		GLenum colorBit = color ? GL_COLOR_BUFFER_BIT : 0;
		GLenum depthBit = depth ? GL_DEPTH_BUFFER_BIT : 0;
		GLenum stencilBit = stencil ? GL_STENCIL_BUFFER_BIT : 0;
		glClear(colorBit | depthBit | stencilBit);
	}

	void setTest(void **data)
	{
		GLint bits; glGetIntegerv(GL_STENCIL_BITS, &bits);
		if (bits < 8) DEBUG_MESSAGE("warning", "BufferSettings - setTest", "stencil < 8", );
		if (CONVERT(bool, 0)) { glEnable(GL_SCISSOR_TEST); }
		else { glDisable(GL_SCISSOR_TEST); }
		if (CONVERT(bool, 1)) { glEnable(GL_STENCIL_TEST); }
		else { glDisable(GL_STENCIL_TEST); }
		if (CONVERT(bool, 2)) { glEnable(GL_DEPTH_TEST); }
		else { glDisable(GL_DEPTH_TEST); }
	}

	void setStencilFunctionFrontFace(void **data)
	{
		glStencilFuncSeparate(GL_FRONT, CONVERT(GLenum, 0), CONVERT(int, 1), CONVERT(uint8_t, 2));
	}

	void setStencilOperationFrontFace(void **data)
	{
		glStencilOpSeparate(GL_FRONT, CONVERT(GLenum, 0), CONVERT(GLenum, 1), CONVERT(GLenum, 2));
	}

	void setStencilFunctionBackFace(void **data)
	{
		glStencilFuncSeparate(GL_BACK, CONVERT(GLenum, 0), CONVERT(int, 1), CONVERT(uint8_t, 2));
	}

	void setStencilOperationBackFace(void **data)
	{
		glStencilOpSeparate(GL_BACK, CONVERT(GLenum, 0), CONVERT(GLenum, 1), CONVERT(GLenum, 2));
	}

	void setStencilFunction(void **data)
	{
		glStencilFunc(CONVERT(GLenum, 0), CONVERT(int, 1), CONVERT(uint8_t, 2));
	}

	void setStencilOperation(void **data)
	{
		glStencilOp(CONVERT(GLenum, 0), CONVERT(GLenum, 1), CONVERT(GLenum, 2));
	}

	void setBlend(void **data)
	{
		if (CONVERT(bool, 1))
			glEnablei(GL_BLEND, CONVERT(int, 0));
		else
			glDisablei(GL_BLEND, CONVERT(int, 0));
	}

	void setBlendEquation(void **data)
	{
		glBlendEquationSeparate(CONVERT(GLenum, 0), CONVERT(GLenum, 1));
	}

	void setBlendEquation(void **data)
	{
		glBlendEquation(CONVERT(GLenum, 0));
	}

	void setBlendFunc(void **data)
	{
		glBlendFuncSeparate(CONVERT(GLenum, 0), CONVERT(GLenum, 1), CONVERT(GLenum, 2), CONVERT(GLenum, 3));
	}

	void setBlendFunc(void **data)
	{
		glBlendFunc(CONVERT(GLenum, 0), CONVERT(GLenum, 1));
	}

	void setBlendConstant(void **data)
	{
		glBlendColor(CONVERT(glm::vec4, 0)[0], CONVERT(glm::vec4, 0)[1], CONVERT(glm::vec4, 0)[2], CONVERT(glm::vec4, 0)[4]);
	}

}