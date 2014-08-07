#include <Render/RenderPass.hh>
#include <cassert>
#include <string>
#include <Render/Shader.hh>
#include <Core/Drawable.hh>
#include <Render/OpenGLTask.hh>
#include <Render/GeometryManager.hh>

# undef DEBUG_MESSAGE
# define DEBUG_MESSAGE(type, from, reason, return_type) \
	{	assert(0 && std::string(std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "].").c_str()); return return_type; }


namespace gl
{
	RenderPass::RenderPass()
		: _stencilSize(-1),
		_rect(glm::ivec4(0, 0, 512, 512)),
		_sample(1),
		_shader(NULL),
		_mode(GL_TRIANGLES),
		_colorOutputTarget(NULL)
	{
	}


	RenderPass::~RenderPass()
	{
		for (size_t index = 0; index < _tasks.size(); ++index)
		{	
			for (uint8_t param = 0; param < _tasks[index].nbrParams; ++param)
				delete _tasks[index].params[param];
			delete[] _tasks[index].params;
		}
		delete[] _colorOutputTarget;
		for (size_t index = 0; index < _colorOutput.size(); ++index)
			delete _colorOutput[index].second;
	}

	RenderPass::RenderPass(RenderPass const &copy)
		: _stencilSize(copy._stencilSize),
		_rect(copy._rect),
		_sample(copy._sample),
		_shader(copy._shader),
		_mode(copy._mode),
		_colorOutputTarget(NULL)
	{
	}

	RenderPass &RenderPass::operator=(RenderPass const &r)
	{
		if (this != &r)
		{
			_stencilSize = r._stencilSize;
			_rect = r._rect;
			_sample = r._sample;
			_shader = r._shader;
			_mode = r._mode;
		}
		return (*this);
	}

	RenderPass &RenderPass::pushSetScissorTask(glm::ivec4 const &area)
	{
		Task task;

		setTaskAllocation(task, area);
		task.func = setScissor;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetClearValueTask(glm::vec4 const &color, float depth, uint8_t stencil)
	{
		Task task;

		setTaskAllocation(task, color, depth, stencil);
		task.func = setClearValue;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetColorMaskTask(GLuint index, glm::bvec4 const &color)
	{
		Task task;

		setTaskAllocation(task, index, color);
		task.func = setColorMask;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetDepthMaskTask(bool depth)
	{
		Task task;

		setTaskAllocation(task, depth);
		task.func = setDepthMask;
		_tasks.push_back(task);
		return (*this);	return (*this);
	}

	RenderPass &RenderPass::pushSetStencilMaskTask(uint8_t front, uint8_t back)
	{
		Task task;

		setTaskAllocation(task, front, back);
		task.func = setStencilMask;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushClearTask(bool color, bool depth, bool stencil)
	{
		Task task;

		setTaskAllocation(task, color, depth, stencil);
		task.func = clear;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetStencilFunctionFrontFaceTask(GLenum func, int ref, uint8_t mask)
	{
		Task task;

		setTaskAllocation(task, func, ref, mask);
		task.func = setStencilFunctionFrontFace;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetStencilOperationFrontFaceTask(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		Task task;

		setTaskAllocation(task, opStencilFail, opDepthFail, opDepthPass);
		task.func = setStencilOperationFrontFace;
		_tasks.push_back(task);
		return (*this);
	}
	
	RenderPass &RenderPass::pushSetStencilFunctionBackFaceTask(GLenum func, int ref, uint8_t mask)
	{
		Task task;
		setTaskAllocation(task, func, ref, mask);
		task.func = setStencilFunctionBackFace;
		_tasks.push_back(task);
		return (*this);
	}
	
	RenderPass &RenderPass::pushSetStencilOperationBackFaceTask(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		Task task;

		setTaskAllocation(task, opStencilFail, opDepthFail, opDepthPass);
		task.func = setStencilOperationBackFace;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetStencilFunctionTask(GLenum func, int ref, uint8_t mask)
	{
		Task task;

		setTaskAllocation(task, func, ref, mask);
		task.func = setStencilFunction;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetStencilOperationTask(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		Task task;

		setTaskAllocation(task, opStencilFail, opDepthFail, opDepthPass);
		task.func = setStencilOperation;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetBlendTask(int drawBuffer, bool state)
	{
		Task task;

		setTaskAllocation(task, drawBuffer, state);
		task.func = setBlend;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetBlendEquationTask(GLenum colorMode, GLenum alphaMode)
	{
		Task task;

		setTaskAllocation(task, colorMode, alphaMode);
		task.func = setBlendEquationSeparate;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetBlendEquationTask(GLenum mode)
	{
		Task task;

		setTaskAllocation(task, mode);
		task.func = setBlendEquation;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetBlendFuncTask(GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha)
	{
		Task task;

		setTaskAllocation(task, srcRGB, destRGB, srcAlpha, destAlpha);
		task.func = setBlendFuncSeparate;
		_tasks.push_back(task);
		return (*this);
	}
	RenderPass &RenderPass::pushSetBlendFuncTask(GLenum src, GLenum dest)
	{
		Task task;

		setTaskAllocation(task, src, dest);
		task.func = setBlendFunc;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetBlendConstantTask(glm::vec4 const &blendColor)
	{
		Task task;

		setTaskAllocation(task, blendColor);
		task.func = setBlendConstant;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetTestTask(bool scissor, bool stencil, bool depth)
	{
		Task task;

		setTaskAllocation(task, scissor, stencil, depth);
		task.func = setTest;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::popTask()
	{
		if (!(_tasks.size() > 0))
			DEBUG_MESSAGE("Warning", "RenderPass - popTask", "No task to pop", *this);
		auto &element = _tasks.back();
		for (uint8_t index = 0; index < element.nbrParams; ++index)
			delete element.params[index];
		delete[] element.params;
		_tasks.pop_back();
		return (*this);
	}

	RenderPass &RenderPass::update()
	{
		if (_shader == NULL)
			DEBUG_MESSAGE("Warning", "RenderPass - update", "shader bind to renderPass assign to NULL", *this);
		_fbo.bind();
		_fbo.size(_rect.z, _rect.w, _sample);
		_fbo.viewPort(_rect);
		for (size_t index = 0; index < _tasks.size(); ++index)
			_tasks[index].func(_tasks[index].params);
		return (*this);
	}

	bool RenderPass::stencilSizeValid()
	{
		if (_stencilSize == -1)
			glGetIntegerv(GL_STENCIL_BITS, &_stencilSize);
		return (_stencilSize < 8) ? false : true;
	}

	RenderPass &RenderPass::config(glm::ivec4 const &rect, GLint sample)
	{
		_rect = rect;
		_sample = sample;
		return (*this);
	}

	RenderPass &RenderPass::bindShader(Shader *shader)
	{
		_shader = shader;
		return (*this);
	}
	
	Shader *RenderPass::accessShader() const
	{
		return (_shader);
	}

	RenderPass &RenderPass::setMode(GLenum mode)
	{
		_mode = mode;
		return (*this);
	}

	GLenum RenderPass::getMode() const
	{
		return (_mode);
	}

	RenderPass &RenderPass::addColorOutput(GLenum target, GLenum internalFormat)
	{
		_colorOutput.push_back(std::make_pair(target, new Texture2D(_rect.z, _rect.w, internalFormat, false)));
		_fbo.bind();
		if (_colorOutputTarget)
			delete _colorOutputTarget;
		_colorOutputTarget = new GLenum[_colorOutput.size()];
		for (size_t index = 0; index < _colorOutput.size(); ++index)
		{
			_colorOutputTarget[index] = _colorOutput[index].first;
			_colorOutput[index].second->attachement(_fbo, _colorOutputTarget[index]);
		}
		glDrawBuffers(_colorOutput.size(), _colorOutputTarget);
		_fbo.unbind();
		return (*this);
	}
}