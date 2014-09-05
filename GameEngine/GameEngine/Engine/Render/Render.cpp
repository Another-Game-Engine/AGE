#include <Render/Render.hh>
#include <cassert>
#include <string>
#include <Core/Drawable.hh>
#include <Render/OpenGLTask.hh>
#include <Render/GeometryManager.hh>
#include <Render/MaterialManager.hh>
#include <Render/Storage.hh>
#

namespace gl
{
	Render::Render(Shader &shader, GeometryManager &g)
		: _rect(0, 0, 512, 512),
		_mode(GL_TRIANGLES),
		_shader(shader),
		_branch(NULL),
		_geometryManager(g)
	{

	}

	Render::~Render()
	{
		for (size_t index = 0; index < _tasks.size(); ++index)
		{	
			for (uint8_t param = 0; param < _tasks[index].nbrParams; ++param)
				delete _tasks[index].params[param];
			delete[] _tasks[index].params;
		}
	}

	Render &Render::pushSetScissorTask(glm::ivec4 const &area)
	{
		Task task;

		setTaskAllocation(task, area);
		task.func = setScissor;
		_tasks.push_back(task);
		return (*this);
	}

	Render &Render::pushSetClearValueTask(glm::vec4 const &color, float depth, uint8_t stencil)
	{
		Task task;

		setTaskAllocation(task, color, depth, stencil);
		task.func = setClearValue;
		_tasks.push_back(task);
		return (*this);
	}

	Render &Render::pushSetColorMaskTask(GLuint index, glm::bvec4 const &color)
	{
		Task task;

		setTaskAllocation(task, index, color);
		task.func = setColorMask;
		_tasks.push_back(task);
		return (*this);
	}

	Render &Render::pushSetDepthMaskTask(bool depth)
	{
		Task task;

		setTaskAllocation(task, depth);
		task.func = setDepthMask;
		_tasks.push_back(task);
		return (*this);	return (*this);
	}

	Render &Render::pushSetStencilMaskTask(uint8_t front, uint8_t back)
	{
		Task task;

		setTaskAllocation(task, front, back);
		task.func = setStencilMask;
		_tasks.push_back(task);
		return (*this);
	}

	Render &Render::pushClearTask(bool color, bool depth, bool stencil)
	{
		Task task;

		setTaskAllocation(task, color, depth, stencil);
		task.func = clear;
		_tasks.push_back(task);
		return (*this);
	}

	Render &Render::pushSetStencilFunctionFrontFaceTask(GLenum func, int ref, uint8_t mask)
	{
		Task task;

		setTaskAllocation(task, func, ref, mask);
		task.func = setStencilFunctionFrontFace;
		_tasks.push_back(task);
		return (*this);
	}

	Render &Render::pushSetStencilOperationFrontFaceTask(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		Task task;

		setTaskAllocation(task, opStencilFail, opDepthFail, opDepthPass);
		task.func = setStencilOperationFrontFace;
		_tasks.push_back(task);
		return (*this);
	}
	
	Render &Render::pushSetStencilFunctionBackFaceTask(GLenum func, int ref, uint8_t mask)
	{
		Task task;
		setTaskAllocation(task, func, ref, mask);
		task.func = setStencilFunctionBackFace;
		_tasks.push_back(task);
		return (*this);
	}
	
	Render &Render::pushSetStencilOperationBackFaceTask(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		Task task;

		setTaskAllocation(task, opStencilFail, opDepthFail, opDepthPass);
		task.func = setStencilOperationBackFace;
		_tasks.push_back(task);
		return (*this);
	}

	Render &Render::pushSetStencilFunctionTask(GLenum func, int ref, uint8_t mask)
	{
		Task task;

		setTaskAllocation(task, func, ref, mask);
		task.func = setStencilFunction;
		_tasks.push_back(task);
		return (*this);
	}

	Render &Render::pushSetStencilOperationTask(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		Task task;

		setTaskAllocation(task, opStencilFail, opDepthFail, opDepthPass);
		task.func = setStencilOperation;
		_tasks.push_back(task);
		return (*this);
	}

	Render &Render::pushSetBlendTask(int drawBuffer, bool state)
	{
		Task task;

		setTaskAllocation(task, drawBuffer, state);
		task.func = setBlend;
		_tasks.push_back(task);
		return (*this);
	}

	Render &Render::pushSetBlendEquationTask(GLenum colorMode, GLenum alphaMode)
	{
		Task task;

		setTaskAllocation(task, colorMode, alphaMode);
		task.func = setBlendEquationSeparate;
		_tasks.push_back(task);
		return (*this);
	}

	Render &Render::pushSetBlendEquationTask(GLenum mode)
	{
		Task task;

		setTaskAllocation(task, mode);
		task.func = setBlendEquation;
		_tasks.push_back(task);
		return (*this);
	}

	Render &Render::pushSetBlendFuncTask(GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha)
	{
		Task task;

		setTaskAllocation(task, srcRGB, destRGB, srcAlpha, destAlpha);
		task.func = setBlendFuncSeparate;
		_tasks.push_back(task);
		return (*this);
	}
	Render &Render::pushSetBlendFuncTask(GLenum src, GLenum dest)
	{
		Task task;

		setTaskAllocation(task, src, dest);
		task.func = setBlendFunc;
		_tasks.push_back(task);
		return (*this);
	}

	Render &Render::pushSetBlendConstantTask(glm::vec4 const &blendColor)
	{
		Task task;

		setTaskAllocation(task, blendColor);
		task.func = setBlendConstant;
		_tasks.push_back(task);
		return (*this);
	}

	Render &Render::pushSetTestTask(bool scissor, bool stencil, bool depth)
	{
		Task task;

		setTaskAllocation(task, scissor, stencil, depth);
		task.func = setTest;
		_tasks.push_back(task);
		return (*this);
	}

	Render &Render::popTask()
	{
		if (!(_tasks.size() > 0))
			assert(0);
		auto &element = _tasks.back();
		for (uint8_t index = 0; index < element.nbrParams; ++index)
			delete element.params[index];
		delete[] element.params;
		_tasks.pop_back();
		return (*this);
	}

	Render &Render::configRect(glm::ivec4 const &rect)
	{
		_rect = rect;
		return (*this);
	}

	Render &Render::setMode(GLenum mode)
	{
		_mode = mode;
		return (*this);
	}

	GLenum Render::getMode() const
	{
		return (_mode);
	}

	Render &Render::branchInput(RenderOffScreen const &input)
	{
		_branch = &input;
		return (*this);
	}

	Render &Render::unBranchInput()
	{
		_branch = NULL;
		return (*this);
	}

	Render &Render::pushInputSampler(Key<Sampler> const &key, GLenum attachement)
	{
		if (_shader.hasSampler(key))
			_inputSamplers.push_back(std::make_pair(key, attachement));
		else
			assert(0);
		return (*this);
	}

	Render &Render::popInputSampler()
	{
		_inputSamplers.pop_back();
		return (*this);
	}

	void Render::updateInput()
	{
		if (_branch != NULL)
		{
			for (size_t index = 0; index < _inputSamplers.size(); ++index)
			{
				Texture2D const *text = NULL;
				if ((text = _branch->getBufferSamplable(_inputSamplers[index].second)) != NULL)
					_shader.setSampler(_inputSamplers[index].first, *text);
			}
		}
	}

	RenderOffScreen::RenderOffScreen(Shader &shader, GeometryManager &g)
		: Render(shader, g),
		_sample(0),
		_updateBuffer(true),
		_updateFrameBuffer(true)
	{
	}

	RenderOffScreen::~RenderOffScreen()
	{
		for (auto &element = _buffer.begin(); element != _buffer.end(); ++element)
			if (element->second.first != NULL) delete element->second.first;
	}

	RenderOffScreen &RenderOffScreen::configSample(GLint sample)
	{
		_sample = sample;
		return (*this);
	}

	RenderOffScreen &RenderOffScreen::pushTarget(GLenum attachement)
	{
		_updateBuffer = true;
		_target.push_back(attachement);
		return (*this);
	}

	RenderOffScreen &RenderOffScreen::popTarget()
	{
		_updateBuffer = true;
		_target.pop_back();
		return (*this);
	}

	RenderOffScreen &RenderOffScreen::createBufferSamplable(GLenum attachement, int x, int y, GLenum internalFormat)
	{
		_updateFrameBuffer = true;
		auto &element = _buffer.find(attachement);
		if (element == _buffer.end())
		{
			auto &save = _buffer[attachement];
			save.first = new Texture2D(x, y, internalFormat);
			save.second = true;
		}
		else
		{
			if (element->second.first != NULL)
				delete element->second.first;
			element->second.first = new Texture2D(x, y, internalFormat);
			element->second.second = true;
		}
		return (*this);
	}
	
	RenderOffScreen &RenderOffScreen::createBufferSamplable(GLenum attachement, GLenum internalFormat)
	{
		createBufferSamplable(attachement, _rect[2], _rect[3], internalFormat);
		return (*this);
	}

	RenderOffScreen &RenderOffScreen::deleteBuffer(GLenum attachement)
	{
		_updateFrameBuffer = true;
		auto &element = _buffer.find(attachement);
		if (element != _buffer.end())
		{
			delete element->second.first;
			element->second.first = NULL;
		}
		return (*this);
	}

	Texture2D const *RenderOffScreen::getBufferSamplable(GLenum attachement) const
	{
		auto &element = _buffer.find(attachement);
		if (element == _buffer.end())
			return (NULL);
		else
		{
			if (element->second.first->getType() == GL_TEXTURE_2D)
				return ((Texture2D const *)element->second.first);
			else
				return (NULL);
		}
	}

	RenderOffScreen &RenderOffScreen::createBufferNotSamplable(GLenum attachement, GLenum internalFormat)
	{
		createBufferNotSamplable(attachement, _rect[2], _rect[3], internalFormat);
		return (*this);
	}

	RenderOffScreen &RenderOffScreen::createBufferNotSamplable(GLenum attachement, int x, int y, GLenum internalFormat)
	{
		_updateFrameBuffer = true;
		auto &element = _buffer.find(attachement);
		if (element == _buffer.end())
		{
			auto &save = _buffer[attachement];
			save.first = new RenderBuffer(x, y, internalFormat);
			save.second = true;
		}
		else
		{
			if (element->second.second != NULL)
				delete element->second.first;
			element->second.first = new RenderBuffer(x, y, internalFormat);
			element->second.second = true;
		}
		return (*this);
	}

	RenderBuffer const *RenderOffScreen::getBufferNotSamplable(GLenum attachement) const
	{
		auto &element = _buffer.find(attachement);
		if (element == _buffer.end())
			return (NULL);
		else
		{
			if (element->second.first->getType() == GL_RENDERBUFFER)
				return ((RenderBuffer const *)element->second.first);
			else
				return (NULL);
		}
	}

	RenderOffScreen &RenderOffScreen::useInputBuffer(GLenum attachement)
	{
		auto &search = _branch->_buffer.find(attachement);
		if (search == _branch->_buffer.end())
			return (*this);
		auto &element = _buffer.find(attachement);
		if (element == _buffer.end())
			return (*this);
		if (element->second.first != NULL)
			delete element->second.first;
		element->second.first = search->second.first;
		element->second.second = false;
		return (*this);
	}

	void RenderOffScreen::updateBuffer()
	{
		_updateBuffer = false;
		glDrawBuffers(GLsizei(_target.size()), _target.data());
	}

	void RenderOffScreen::updateFrameBuffer()
	{
		_updateFrameBuffer = false;
		for (auto &element = _buffer.begin(); element != _buffer.end(); ++element)
			_fbo.attachement(*element->second.first, element->first);
	}

	void RenderOffScreen::updateOutput()
	{
		if (_updateBuffer)
			updateBuffer();
		if (_updateFrameBuffer)
			updateFrameBuffer();
	}

	RenderPass::RenderPass(Shader &shader, GeometryManager &g, MaterialManager &m)
		: RenderOffScreen(shader, g),
		_objectsToRender(NULL),
		_materialManager(m)
	{
	}

	RenderPass::~RenderPass()
	{
	}

	RenderPass &RenderPass::setRenderPassObjects(AGE::Vector<AGE::Drawable> const &objects)
	{
		_objectsToRender = &objects;
		return (*this);
	}

	Render &RenderPass::draw()
	{
		_fbo.bind();
		updateOutput();
		updateInput();
		for (size_t index = 0; index < _tasks.size(); ++index)
			_tasks[index].func(_tasks[index].params);
		if (_objectsToRender == NULL)
			return (*this);
		for (size_t index = 0; index < _objectsToRender->size(); ++index)
		{
			AGE::Drawable const &object = (*_objectsToRender)[index];
			_materialManager.setShader(object.material, _shader);
			_shader.update(object.transformation);
			_geometryManager.draw(_mode, object.mesh.indices, object.mesh.vertices);
		}
		_fbo.unbind();
		return (*this);
	}

	RenderPostEffect::RenderPostEffect(Key<Vertices> const &key, Shader &s, GeometryManager &g)
		: RenderOffScreen(s, g),
		_quad(key)
	{

	}

	RenderPostEffect::~RenderPostEffect()
	{

	}

	Render &RenderPostEffect::draw()
	{
		_fbo.bind();
		updateOutput();
		updateInput();
		for (size_t index = 0; index < _tasks.size(); ++index)
			_tasks[index].func(_tasks[index].params);
		_shader.update();
		_geometryManager.draw(_mode, _quad);
		_fbo.unbind();
		return (*this);
	}

	RenderOnScreen::RenderOnScreen(Key<Vertices> const &key, Shader &s, GeometryManager &g)
		: Render(s, g),
		_quad(key)
	{

	}

	RenderOnScreen::~RenderOnScreen()
	{

	}

	Render &RenderOnScreen::draw()
	{
		updateInput();
		for (size_t index = 0; index < _tasks.size(); ++index)
			_tasks[index].func(_tasks[index].params);
		_shader.update();
		_geometryManager.draw(_mode, _quad);
		return (*this);
	}

	RenderType RenderOnScreen::getType() const
	{
		return (RenderType::RENDER_ON_SCREEN);
	}

	RenderType RenderPostEffect::getType() const
	{
		return (RenderType::RENDER_POST_EFFECT);
	}

	RenderType RenderPass::getType() const
	{
		return (RenderType::RENDER_PASS);
	}
}