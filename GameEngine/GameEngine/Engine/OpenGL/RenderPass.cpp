#include <OpenGL/RenderPass.hh>
#include <cassert>
#include <string>
#include <OpenGL/Shader.hh>
#include <OpenGL/Drawable.hh>
#include <OpenGL/BufferSettings.hh>

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
		for (size_t index = 0; index < _tasks.size(); ++index)
		{	
			for (uint8_t param = 0; param < _tasks[index].nbrParams; ++param)
				delete _tasks[index].params[param];
			delete[] _tasks[index].params;
		}
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

	RenderPass &RenderPass::pushSetScissorTask(glm::ivec4 const &area)
	{
		Task task;
		setAllocation(task, area);
		task.func = setScissor;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetClearValueTask(glm::vec4 const &color, float depth, uint8_t stencil)
	{
		Task task;
		setAllocation(task, color, depth, stencil);
		task.func = setClearValue;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetColorMaskTask(GLuint index, glm::bvec4 const &color)
	{
		Task task;
		setAllocation(task, index, color);
		task.func = setColorMask;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetDepthMaskTask(bool depth)
	{
		Task task;
		setAllocation(task, depth);
		task.func = setDepthMask;
		_tasks.push_back(task);
		return (*this);	return (*this);
	}

	RenderPass &RenderPass::pushSetStencilMaskTask(uint8_t front, uint8_t back)
	{
		Task task;
		setAllocation(task, front, back);
		task.func = setStencilMask;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushClearTask(bool color, bool depth, bool stencil)
	{
		Task task;
		setAllocation(task, color, depth, stencil);
		task.func = clear;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetStencilFunctionFrontFaceTask(GLenum func, int ref, uint8_t mask)
	{
		Task task;
		setAllocation(task, func, ref, mask);
		task.func = setStencilFunctionFrontFace;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetStencilOperationFrontFaceTask(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		Task task;
		setAllocation(task, opStencilFail, opDepthFail, opDepthPass);
		task.func = setStencilOperationFrontFace;
		_tasks.push_back(task);
		return (*this);
	}
	
	RenderPass &RenderPass::pushSetStencilFunctionBackFaceTask(GLenum func, int ref, uint8_t mask)
	{
		Task task;
		setAllocation(task, func, ref, mask);
		task.func = setStencilFunctionBackFace;
		_tasks.push_back(task);
		return (*this);
	}
	
	RenderPass &RenderPass::pushSetStencilOperationBackFaceTask(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		Task task;
		setAllocation(task, opStencilFail, opDepthFail, opDepthPass);
		task.func = setStencilOperationBackFace;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetStencilFunctionTask(GLenum func, int ref, uint8_t mask)
	{
		Task task;
		setAllocation(task, func, ref, mask);
		task.func = setStencilFunction;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetStencilOperationTask(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		Task task;
		setAllocation(task, opStencilFail, opDepthFail, opDepthPass);
		task.func = setStencilOperation;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetBlendTask(int drawBuffer, bool state)
	{
		Task task;
		setAllocation(task, drawBuffer, state);
		task.func = setBlend;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetBlendEquationTask(GLenum colorMode, GLenum alphaMode)
	{
		Task task;
		setAllocation(task, colorMode, alphaMode);
		task.func = setBlendEquationSeparate;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetBlendEquationTask(GLenum mode)
	{
		Task task;
		setAllocation(task, mode);
		task.func = setBlendEquation;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetBlendFuncTask(GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha)
	{
		Task task;
		setAllocation(task, srcRGB, destRGB, srcAlpha, destAlpha);
		task.func = setBlendFuncSeparate;
		_tasks.push_back(task);
		return (*this);
	}
	RenderPass &RenderPass::pushSetBlendFuncTask(GLenum src, GLenum dest)
	{
		Task task;
		setAllocation(task, src, dest);
		task.func = setBlendFunc;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetBlendConstantTask(glm::vec4 const &blendColor)
	{
		Task task;
		setAllocation(task, blendColor);
		task.func = setBlendConstant;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::pushSetTestTask(bool scissor, bool stencil, bool depth)
	{
		Task task;
		setAllocation(task, scissor, stencil, depth);
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

	RenderPass &RenderPass::draw(Drawable const *objectRender, size_t nbrObjectRender)
	{
		for (size_t index = 0; index < _tasks.size(); ++index)
			_tasks[index].func(_tasks[index].params);
		if (!_shader)
			DEBUG_MESSAGE("Warning", "RenderPass - use", "no shader assign on this renderPass", *this);
		_shader->use();
		return (*this);
	}

	
}