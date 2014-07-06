#include <OpenGL/RenderPass.hh>
#include <cassert>
#include <string>

# undef DEBUG_MESSAGE
# define DEBUG_MESSAGE(type, from, reason, return_type) \
	{	assert(0 && std::string(std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "].").c_str()); return return_type; }


namespace gl
{
	RenderPass::RenderPass()
		: _scissorTest(false),
		_multisampling(false),
		_leftScissor(0),
		_bottomScissor(0),
		_widthScissor(0),
		_heightScissor(0),
		_red(0.0f),
		_green(0.0f),
		_blue(0.0f),
		_alpha(0.0f),
		_depth(1.0f),
		_stencil(1),
		_maskDepth(GL_TRUE),
		_maskStencilBack(GL_TRUE),
		_maskStencilFront(GL_TRUE)
	{

	}

	RenderPass::~RenderPass()
	{

	}

	RenderPass::RenderPass(RenderPass const &copy)
		: _scissorTest(copy._scissorTest),
		_leftScissor(copy._leftScissor),
		_bottomScissor(copy._bottomScissor),
		_widthScissor(copy._widthScissor),
		_heightScissor(copy._heightScissor)
	{
	}

	RenderPass &RenderPass::operator=(RenderPass const &r)
	{
		if (this == &r)
		{
			_scissorTest = r._scissorTest;
			_leftScissor = r._leftScissor;
			_bottomScissor = r._bottomScissor;
			_widthScissor = r._widthScissor;
			_heightScissor = r._heightScissor;
		}
		return (*this);
	}

	RenderPass &RenderPass::setScissorTest(bool state)
	{
		_scissorTest = state;
		return (*this);
	}

	RenderPass &RenderPass::setMultiSampling(bool state, float ratio)
	{
		GLint param; glGetIntegerv(GL_SAMPLE_BUFFERS, &param);
		if (param != 1)
			DEBUG_MESSAGE("Warning", "RenderPass - setMultiSampling", "No multisampler possible", *this);
		_multisampling = state;
		_multisampleRatio = ratio;
		return (*this);
	}

	RenderPass &RenderPass::setScissor(GLint left, GLint bottom, GLsizei width, GLsizei height)
	{
		_leftScissor = left;
		_bottomScissor = bottom;
		_widthScissor = width;
		_heightScissor = height;
		return (*this);
	};

	RenderPass &RenderPass::setColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
	{
		_red = red;
		_blue = blue;
		_green = green;
		_alpha = alpha;
		return (*this);
	}

	RenderPass &RenderPass::setDepth(GLclampd depth)
	{
		_depth = depth;
		return (*this);
	}

	RenderPass &RenderPass::setStencil(GLint stencil)
	{
		_stencil = stencil;
		return (*this);
	}

	RenderPass &RenderPass::setMaskColor(GLuint index, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
	{
		_maskRed[index] = red;
		_maskGreen[index] = green;
		_maskBlue[index] = blue;
		_maskAlpha[index] = green;
		return (*this);
	}

	RenderPass &RenderPass::setMaskDepth(GLboolean depth)
	{
		_maskDepth = depth;
		return (*this);
	}

	RenderPass &RenderPass::setStencilMaskBackFace(GLboolean state)
	{
		_maskStencilBack = state;
		return (*this);
	}

	RenderPass &RenderPass::setStencilMaskFrontFace(GLboolean state)
	{
		_maskStencilFront = state;
		return (*this);
	}

	RenderPass &RenderPass::setClearColor(bool state)
	{
		_clearColor = state ? GL_COLOR_BUFFER_BIT : 0;
		return (*this);
	}

	RenderPass &RenderPass::setClearDepth(bool state)
	{
		_clearDepth = state ? GL_DEPTH_BUFFER_BIT : 0;
		return (*this);
	}

	RenderPass &RenderPass::setClearStencil(bool state)
	{
		_clearStencil = state ? GL_STENCIL_BUFFER_BIT : 0;
		return (*this);
	}

	RenderPass &RenderPass::use()
	{
		glClear(_clearColor | _clearDepth | _clearStencil);
		if (_multisampling)
		{
			glEnable(GL_MULTISAMPLE);
			glEnable(GL_SAMPLE_SHADING);
			glMinSampleShading(_multisampleRatio);
		}
		glClearColor(_red, _green, _blue, _alpha);
		glClearDepth(_depth);
		glClearStencil(_stencil);
		for (GLuint index = 0; index < GL_MAX_COLOR_ATTACHMENTS; ++index)
			glColorMaski(index, _maskRed[index], _maskGreen[index], _maskBlue[index], _maskAlpha[index]);
		glDepthMask(_maskDepth);
		glStencilMaskSeparate(GL_FRONT, _maskStencilFront);
		glStencilMaskSeparate(GL_BACK, _maskStencilBack);
		if (_scissorTest) glScissor(_leftScissor, _bottomScissor, _widthScissor, _heightScissor);
		return (*this);
	}

}