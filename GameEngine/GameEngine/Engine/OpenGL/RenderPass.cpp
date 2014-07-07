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
		: _shader(NULL),
		_clearColor(0),
		_clearDepth(0),
		_clearStencil(0),
		_color(glm::vec4(0.f, 0.f, 0.f, 1.f)),
		_depth(1.f),
		_stencil(0),
		_maskDepth(true),
		_maskStencilBack(0xFF),
		_maskStencilFront(0xFF),
		_scissorTest(false),
		_stencilTest(false),
		_depthTest(true),
		_faceBackStencil(GL_ALWAYS, 0, 0xff, GL_KEEP, GL_KEEP, GL_KEEP),
		_faceFrontStencil(GL_ALWAYS, 0, 0xff, GL_KEEP, GL_KEEP, GL_KEEP)
	{
		for (GLint index = 0; index < GL_MAX_COLOR_ATTACHMENTS; ++index)
			_maskColor[index] = glm::bvec4(true, true, true, true);
	}

	RenderPass::RenderPass(Shader &shader)
		: RenderPass()
	{
		_shader = &shader;
		for (GLint index = 0; index < GL_MAX_COLOR_ATTACHMENTS; ++index)
			_maskColor[index] = glm::bvec4(true, true, true, true);
	}

	RenderPass::~RenderPass()
	{
	}

	RenderPass::RenderPass(RenderPass const &copy)
		: _shader(copy._shader),
		_clearColor(copy._clearColor),
		_clearDepth(copy._clearDepth),
		_clearStencil(copy._clearStencil),
		_color(copy._color),
		_depth(copy._depth),
		_stencil(copy._stencil),
		_maskDepth(copy._maskDepth),
		_maskStencilBack(copy._maskStencilBack),
		_maskStencilFront(copy._maskStencilFront)
	{
		for (GLint index = 0; index < GL_MAX_COLOR_ATTACHMENTS; ++index)
			_maskColor[index] = copy._maskColor[index];
	}

	RenderPass &RenderPass::operator=(RenderPass const &r)
	{
		if (this != &r)
		{
			_shader = r._shader;
			_clearColor = r._clearColor;
			_clearDepth = r._clearDepth;
			_clearStencil = r._clearStencil;
			_color = r._color;
			_depth = r._depth;
			_stencil = r._stencil;
			_maskDepth = r._maskDepth;
			_maskStencilBack = r._maskStencilBack;
			_maskStencilFront = r._maskStencilFront;
			for (GLint index = 0; index < GL_MAX_COLOR_ATTACHMENTS; ++index)
				_maskColor[index] = r._maskColor[index];
		}
		return (*this);
	}

	RenderPass &RenderPass::setScissorArea(glm::ivec4 const &area)
	{
		_area = area;
		return (*this);
	}

	RenderPass &RenderPass::setClearValue(glm::vec4 const &color, float depth, unsigned char stencil)
	{
		_color = color;
		_depth = depth;
		_stencil = stencil;
		return (*this);
	}

	RenderPass &RenderPass::setColorMask(GLuint index, glm::bvec4 const &color)
	{
		_maskColor[index] = color;
		return (*this);
	}

	RenderPass &RenderPass::setDepthStencilMask(bool depth, uint8_t front, uint8_t back)
	{
		_maskDepth = depth;
		_maskStencilFront = front;
		_maskStencilBack = back;
		return (*this);
	}

	RenderPass &RenderPass::setClearOption(bool color, bool depth, bool stencil)
	{
		_clearColor = color ? GL_COLOR_BUFFER_BIT : 0;
		_clearDepth = depth ? GL_DEPTH_BUFFER_BIT : 0;
		_clearStencil = stencil ? GL_STENCIL_BUFFER_BIT : 0;
		return (*this);
	}

	RenderPass &RenderPass::setTest(bool scissor, bool stencil, bool depth)
	{
		_scissorTest = scissor;
		_stencilTest = stencil;
		_depthTest = depth;
		return (*this);
	}

	RenderPass &RenderPass::setStencilFunctionFrontFace(GLenum func, int ref, uint8_t mask)
	{
		std::get<0>(_faceFrontStencil) = func;
		std::get<1>(_faceFrontStencil) = ref;
		std::get<2>(_faceFrontStencil) = mask;
		return (*this);
	}

	RenderPass &RenderPass::setStencilOperationFrontFace(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		std::get<3>(_faceFrontStencil) = opStencilFail;
		std::get<4>(_faceFrontStencil) = opDepthFail;
		std::get<5>(_faceFrontStencil) = opDepthPass;
		return (*this);
	}

	RenderPass &RenderPass::setStencilFunctionBackFace(GLenum func, int ref, uint8_t mask)
	{
		std::get<0>(_faceBackStencil) = func;
		std::get<1>(_faceBackStencil) = ref;
		std::get<2>(_faceBackStencil) = mask;
		return (*this);
	}

	RenderPass &RenderPass::setStencilOperationBackFace(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		std::get<3>(_faceBackStencil) = opStencilFail;
		std::get<4>(_faceBackStencil) = opDepthFail;
		std::get<5>(_faceBackStencil) = opDepthPass;
		return (*this);
	}

	RenderPass &RenderPass::setStencilFunction(GLenum func, int ref, uint8_t mask)
	{
		std::get<0>(_faceFrontStencil) = func;
		std::get<1>(_faceFrontStencil) = ref;
		std::get<2>(_faceFrontStencil) = mask;
		std::get<0>(_faceBackStencil) = func;
		std::get<1>(_faceBackStencil) = ref;
		std::get<2>(_faceBackStencil) = mask;
		return (*this);
	}

	RenderPass &RenderPass::setStencilOperation(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		std::get<3>(_faceFrontStencil) = opStencilFail;
		std::get<4>(_faceFrontStencil) = opDepthFail;
		std::get<5>(_faceFrontStencil) = opDepthPass;
		std::get<3>(_faceBackStencil) = opStencilFail;
		std::get<4>(_faceBackStencil) = opDepthFail;
		std::get<5>(_faceBackStencil) = opDepthPass;
		return (*this);
	}
	
	RenderPass &RenderPass::use()
	{
		if (_scissorTest) { glEnable(GL_SCISSOR_TEST); glScissor(_area[0], _area[1], _area[2], _area[3]); }
		if (_stencilTest) 
		{ 
			glEnable(GL_STENCIL_TEST);
			// set stencil
			glStencilMaskSeparate(GL_FRONT, _maskStencilFront);
			glStencilMaskSeparate(GL_BACK, _maskStencilBack);
			glStencilFuncSeparate(GL_BACK, std::get<0>(_faceBackStencil), std::get<1>(_faceBackStencil), std::get<2>(_faceBackStencil));
			glStencilOpSeparate(GL_BACK, std::get<3>(_faceBackStencil), std::get<4>(_faceBackStencil), std::get<5>(_faceBackStencil));
			glStencilFuncSeparate(GL_FRONT, std::get<0>(_faceFrontStencil), std::get<1>(_faceFrontStencil), std::get<2>(_faceFrontStencil));
			glStencilOpSeparate(GL_FRONT, std::get<3>(_faceFrontStencil), std::get<4>(_faceFrontStencil), std::get<5>(_faceFrontStencil));
		}
		if (_depthTest) { glEnable(GL_DEPTH_TEST); glDepthMask(_maskDepth); }

		// set clear setup
		glClearColor(_color[0], _color[1], _color[2], _color[3]);
		glClearDepth(_depth);
		glClearStencil(_stencil);

		// set color mask
		for (GLuint index = 0; index < GL_MAX_COLOR_ATTACHMENTS; ++index)
			glColorMaski(index, _maskColor[index][0], _maskColor[index][1], _maskColor[index][2], _maskColor[index][3]);

		// clear
		glClear(_clearColor | _clearDepth | _clearStencil);

		if (!_shader)
			DEBUG_MESSAGE("Warning", "RenderPass - use", "no shader assign on this renderPass", *this);
		_shader->use();
		return (*this);
	}
}