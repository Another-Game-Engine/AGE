#include <Render/Render.hh>
#include <cassert>
#include <string>
#include <Core/PreparableObject.hh>
#include <Render/Task.hh>
#include <Render/GeometryManager.hh>
#include <Render/MaterialManager.hh>
#include <Render/Storage.hh>

namespace gl
{
	BaseRender::BaseRender()
		: _rect(0, 0, 800, 600)
	{

	}

	BaseRender::~BaseRender()
	{

	}

	BaseRender &BaseRender::configRect(glm::ivec4 const &rect)
	{
		_rect = rect;
		return (*this);
	}


	void BaseRender::update()
	{
		glScissor(_rect.x, _rect.y, _rect.z, _rect.w);
		glViewport(_rect.x, _rect.y, _rect.z, _rect.w);
	}

	OperationBuffer::OperationBuffer(LocationStorage &locationStorage)
		: _locationStorage(locationStorage)
	{

	}

	OperationBuffer::~OperationBuffer()
	{
		for (size_t index = 0; index < _tasks.size(); ++index)
		{
			for (uint8_t param = 0; param < _tasks[index].nbrParams; ++param)
				delete _tasks[index].params[param];
			delete[] _tasks[index].params;
		}
		for (size_t index = 0; index < _ownFunction.size(); ++index)
			delete _ownFunction[index];
	}

	OperationBuffer &OperationBuffer::pushSetScissorTask(glm::ivec4 const &area)
	{
		Task task;

		setTaskAllocation(task, area);
		task.func = setScissor;
		_tasks.push_back(task);
		return (*this);
	}

	OperationBuffer &OperationBuffer::pushSetClearValueTask(glm::vec4 const &color, float depth, uint8_t stencil)
	{
		Task task;

		setTaskAllocation(task, color, depth, stencil);
		task.func = setClearValue;
		_tasks.push_back(task);
		return (*this);
	}

	OperationBuffer &OperationBuffer::pushSetColorMaskTask(GLuint index, glm::bvec4 const &color)
	{
		Task task;

		setTaskAllocation(task, index, color);
		task.func = setColorMask;
		_tasks.push_back(task);
		return (*this);
	}

	OperationBuffer &OperationBuffer::pushSetDepthMaskTask(bool depth)
	{
		Task task;

		setTaskAllocation(task, depth);
		task.func = setDepthMask;
		_tasks.push_back(task);
		return (*this);	return (*this);
	}

	OperationBuffer &OperationBuffer::pushSetStencilMaskTask(uint8_t front, uint8_t back)
	{
		Task task;

		setTaskAllocation(task, front, back);
		task.func = setStencilMask;
		_tasks.push_back(task);
		return (*this);
	}

	OperationBuffer &OperationBuffer::pushClearTask(bool color, bool depth, bool stencil)
	{
		Task task;

		setTaskAllocation(task, color, depth, stencil);
		task.func = clear;
		_tasks.push_back(task);
		return (*this);
	}

	OperationBuffer &OperationBuffer::pushSetStencilFunctionFrontFaceTask(GLenum func, int ref, uint8_t mask)
	{
		Task task;

		setTaskAllocation(task, func, ref, mask);
		task.func = setStencilFunctionFrontFace;
		_tasks.push_back(task);
		return (*this);
	}

	OperationBuffer &OperationBuffer::pushSetStencilOperationFrontFaceTask(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		Task task;

		setTaskAllocation(task, opStencilFail, opDepthFail, opDepthPass);
		task.func = setStencilOperationFrontFace;
		_tasks.push_back(task);
		return (*this);
	}

	OperationBuffer &OperationBuffer::pushSetStencilFunctionBackFaceTask(GLenum func, int ref, uint8_t mask)
	{
		Task task;
		setTaskAllocation(task, func, ref, mask);
		task.func = setStencilFunctionBackFace;
		_tasks.push_back(task);
		return (*this);
	}

	OperationBuffer &OperationBuffer::pushSetStencilOperationBackFaceTask(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		Task task;

		setTaskAllocation(task, opStencilFail, opDepthFail, opDepthPass);
		task.func = setStencilOperationBackFace;
		_tasks.push_back(task);
		return (*this);
	}

	OperationBuffer &OperationBuffer::pushSetStencilFunctionTask(GLenum func, int ref, uint8_t mask)
	{
		Task task;

		setTaskAllocation(task, func, ref, mask);
		task.func = setStencilFunction;
		_tasks.push_back(task);
		return (*this);
	}

	OperationBuffer &OperationBuffer::pushSetStencilOperationTask(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)
	{
		Task task;

		setTaskAllocation(task, opStencilFail, opDepthFail, opDepthPass);
		task.func = setStencilOperation;
		_tasks.push_back(task);
		return (*this);
	}

	OperationBuffer &OperationBuffer::pushSetBlendTask(int drawBuffer, bool state)
	{
		Task task;

		setTaskAllocation(task, drawBuffer, state);
		task.func = setBlend;
		_tasks.push_back(task);
		return (*this);
	}

	OperationBuffer &OperationBuffer::pushSetBlendEquationTask(GLenum colorMode, GLenum alphaMode)
	{
		Task task;

		setTaskAllocation(task, colorMode, alphaMode);
		task.func = setBlendEquationSeparate;
		_tasks.push_back(task);
		return (*this);
	}

	OperationBuffer &OperationBuffer::pushSetBlendEquationTask(GLenum mode)
	{
		Task task;

		setTaskAllocation(task, mode);
		task.func = setBlendEquation;
		_tasks.push_back(task);
		return (*this);
	}

	OperationBuffer &OperationBuffer::pushSetBlendFuncTask(GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha)
	{
		Task task;

		setTaskAllocation(task, srcRGB, destRGB, srcAlpha, destAlpha);
		task.func = setBlendFuncSeparate;
		_tasks.push_back(task);
		return (*this);
	}
	OperationBuffer &OperationBuffer::pushSetBlendFuncTask(GLenum src, GLenum dest)
	{
		Task task;

		setTaskAllocation(task, src, dest);
		task.func = setBlendFunc;
		_tasks.push_back(task);
		return (*this);
	}

	OperationBuffer &OperationBuffer::pushSetBlendConstantTask(glm::vec4 const &blendColor)
	{
		Task task;

		setTaskAllocation(task, blendColor);
		task.func = setBlendConstant;
		_tasks.push_back(task);
		return (*this);
	}

	OperationBuffer &OperationBuffer::pushSetTestTask(bool scissor, bool stencil, bool depth)
	{
		Task task;

		setTaskAllocation(task, scissor, stencil, depth);
		task.func = setTest;
		_tasks.push_back(task);
		return (*this);
	}

	OperationBuffer &OperationBuffer::pushOwnTask(std::function<void(LocationStorage &)> const &f)
	{
		Task task;

		_ownFunction.push_back(new std::function<void(LocationStorage &)>(f));
		setTaskAllocation(task, _ownFunction.back(), ((LocationStorage *)&_locationStorage));
		task.func = ownTask;
		_tasks.push_back(task);
		return (*this);
	}

	OperationBuffer &OperationBuffer::popTask()
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

	void OperationBuffer::update()
	{
		for (size_t index = 0; index < _tasks.size(); ++index)
			_tasks[index].func(_tasks[index].params);
	}

	OffScreenRender::OffScreenRender(LocationStorage &locationStorage)
		: OperationBuffer(locationStorage),
		BaseRender(),
		_sample(0),
		_updateTarget(true),
		_updateFrameBuffer(true)
	{

	}

	OffScreenRender::~OffScreenRender()
	{
		for (auto &element = _buffer.begin(); element != _buffer.end(); ++element)
			if (element->second.first != NULL && !element->first) 
				delete element->second.first;
	}

	OffScreenRender &OffScreenRender::configSample(GLint sample)
	{
		_sample = sample;
		return (*this);
	}

	OffScreenRender &OffScreenRender::pushTarget(GLenum attachement)
	{
		_updateTarget = true;
		_target.push_back(attachement);
		return (*this);
	}

	OffScreenRender &OffScreenRender::popTarget()
	{
		_updateTarget = true;
		_target.pop_back();
		return (*this);
	}

	OffScreenRender &OffScreenRender::createBufferSamplable(GLenum attachement, int x, int y, GLenum internalFormat)
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

	OffScreenRender &OffScreenRender::createBufferSamplable(GLenum attachement, GLenum internalFormat)
	{
		createBufferSamplable(attachement, _rect[2], _rect[3], internalFormat);
		return (*this);
	}

	OffScreenRender &OffScreenRender::deleteBuffer(GLenum attachement)
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

	Texture2D const *OffScreenRender::getBufferSamplable(GLenum attachement) const
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

	OffScreenRender &OffScreenRender::createBufferNotSamplable(GLenum attachement, GLenum internalFormat)
	{
		createBufferNotSamplable(attachement, _rect[2], _rect[3], internalFormat);
		return (*this);
	}

	OffScreenRender &OffScreenRender::createBufferNotSamplable(GLenum attachement, int x, int y, GLenum internalFormat)
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

	RenderBuffer const *OffScreenRender::getBufferNotSamplable(GLenum attachement) const
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

	OffScreenRender &OffScreenRender::useInputBuffer(GLenum attachement, OffScreenRender const &render)
	{
		auto &search = render._buffer.find(attachement);
		if (search == render._buffer.end())
			return (*this);
		auto &element = _buffer[attachement];
		if (element.first != NULL)
			delete element.first;
		element.first = search->second.first;
		element.second = false;
		return (*this);
	}

	void OffScreenRender::update()
	{
		BaseRender::update();
		_fbo.bind();
		if (_updateTarget)
		{
			_updateTarget = false;
			glDrawBuffers(GLsizei(_target.size()), _target.data());
		}
		if (_updateFrameBuffer)
		{
			_updateFrameBuffer = false;
			for (auto &element = _buffer.begin(); element != _buffer.end(); ++element)
				_fbo.attachement(*element->second.first, element->first);
		}
		OperationBuffer::update();
	}

	DrawableRender::DrawableRender(Shader &shader, GeometryManager &geo)
		: _geometryManager(geo),
		_shader(shader),
		_mode(GL_TRIANGLES)
	{

	}

	DrawableRender::~DrawableRender()
	{

	}

	DrawableRender &DrawableRender::setMode(GLenum mode)
	{
		_mode = mode;
		return (*this);
	}


	GLenum DrawableRender::getMode() const
	{
		return (_mode);
	}

	DrawableRender &DrawableRender::pushInputSampler(Key<Sampler> const &key, GLenum attachement, OffScreenRender const &render)
	{
		if (_shader.hasSampler(key))
			_inputSamplers.push_back(Input(key, attachement, render));
		else
			assert(0);
		return (*this);
	}

	DrawableRender &DrawableRender::popInputSampler()
	{
		_inputSamplers.pop_back();
		return (*this);
	}

	void DrawableRender::update()
	{
		_shader.use();
		for (size_t index = 0; index < _inputSamplers.size(); ++index)
		{
			Texture2D const *text = NULL;
			if ((text = _inputSamplers[index].render.getBufferSamplable(_inputSamplers[index].attachement)) != NULL)
				_shader.setSampler(_inputSamplers[index].sampler, *text);
		}
	}

	DrawableRender::Input::Input(Key<Sampler> const &sampler, GLenum attachement, OffScreenRender const &render)
		: sampler(sampler),
		attachement(attachement),
		render(render)
	{

	}

	DrawableRender::Input::~Input()
	{

	}

	DrawableRender::Input::Input(DrawableRender::Input const &copy)
		: sampler(copy.sampler),
		attachement(copy.attachement),
		render(copy.render)
	{

	}

	RenderOnScreen::RenderOnScreen(Key<Vertices> const &key, Key<Indices> const &id, Shader &s, GeometryManager &g, LocationStorage &l)
		: DrawableRender(s, g),
		OperationBuffer(l),
		QuadRender(key, id),
		BaseRender()
	{

	}

	RenderOnScreen::~RenderOnScreen()
	{

	}

	BaseRender &RenderOnScreen::render()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK);
		BaseRender::update();
		DrawableRender::update();
		OperationBuffer::update();
		_shader.update();
		_geometryManager.draw(_mode, _id, _vertices);
		return (*this);
	}

	RenderType RenderOnScreen::getType() const
	{
		return (RenderType::RENDER_ON_SCREEN);
	}

	RenderPass::RenderPass(Shader &s, GeometryManager &g, MaterialManager &m, LocationStorage &l)
		: DrawableRender(s, g),
		OffScreenRender(l),
		_materialManager(m),
		_toRender(NULL),
		_start(0),
		_end(0)
	{
	}

	RenderPass::~RenderPass()
	{
	}

	RenderPass &RenderPass::pushPassTask()
	{
		Task task;

		setTaskAllocation(task, &_geometryManager, &_materialManager, &_shader, &_toRender, &_mode, &_start, &_end);
		task.func = draw;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::setPass(AGE::Vector<AGE::Drawable> const &objects, size_t start, size_t end)
	{
		_toRender = &objects;
		_start = start;
		_end = end;
		return (*this);
	}

	RenderPass &RenderPass::setPass()
	{
		_toRender = NULL;
		_start = 0;
		_end = 0;
		return (*this);
	}

	BaseRender &RenderPass::render()
	{
		_fbo.bind();
		_shader.use();
		DrawableRender::update();
		OffScreenRender::update();
		return (*this);
	}

	RenderType RenderPass::getType() const
	{
		return (RenderType::RENDER_PASS);
	}

	RenderPostEffect::RenderPostEffect(Key<Vertices> const &key, Key<Indices> const &id, Shader &s, GeometryManager &g, LocationStorage &l)
		: DrawableRender(s, g),
		OffScreenRender(l),
		QuadRender(key, id)
	{

	}

	RenderPostEffect::~RenderPostEffect()
	{

	}

	BaseRender &RenderPostEffect::render()
	{
		_fbo.bind();
		_shader.use();
		DrawableRender::update();
		OffScreenRender::update();
		_shader.update();
		_geometryManager.draw(_mode, _id, _vertices);
		return (*this);
	}

	RenderType RenderPostEffect::getType() const
	{
		return (RenderType::RENDER_POST_EFFECT);
	}

	EmptyRenderPass::EmptyRenderPass(LocationStorage &locationStorage)
		: OffScreenRender(locationStorage)
	{

	}

	EmptyRenderPass::~EmptyRenderPass()
	{

	}

	BaseRender &EmptyRenderPass::render()
	{
		_fbo.bind();
		OffScreenRender::update();
		return (*this);
	}

	RenderType EmptyRenderPass::getType() const
	{
		return (RenderType::RENDER_EMPTY);
	}

	QuadRender::QuadRender(Key<Vertices> const &key, Key<Indices> const &id)
		: _vertices(key),
		_id(id)
	{

	}

	QuadRender::~QuadRender()
	{

	}
}