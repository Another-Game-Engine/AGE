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
	Render::Render(Draw *draw)
		: _rect(0, 0, 512, 512),
		_draw(*draw)
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
		delete &_draw;
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
		_draw.mode = mode;
		return (*this);
	}


	GLenum Render::getMode() const
	{
		return (_draw.mode);
	}

	Render &Render::pushInputSampler(Key<Sampler> const &key, GLenum attachement, RenderOffScreen const &render)
	{
		if (_draw.shader.hasSampler(key))
			_inputSamplers.push_back(Input(key, attachement, render));
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
		for (size_t index = 0; index < _inputSamplers.size(); ++index)
		{
			Texture2D const *text = NULL;
			if ((text = _inputSamplers[index].render.getBufferSamplable(_inputSamplers[index].attachement)) != NULL)
				_draw.shader.setSampler(_inputSamplers[index].sampler, *text);
		}
	}

	RenderOffScreen::RenderOffScreen(Render::Draw *draw)
		: Render(draw),
		_sample(0),
		_updateBuffer(true),
		_updateFrameBuffer(true)
	{
	}

	RenderOffScreen::~RenderOffScreen()
	{
		for (auto &element = _buffer.begin(); element != _buffer.end(); ++element)
			if (element->second.first != NULL && !element->first) delete element->second.first;
		for (size_t index = 0; index < _ownFunction.size(); ++index)
			delete _ownFunction[index];
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

	RenderOffScreen &RenderOffScreen::useInputBuffer(GLenum attachement, RenderOffScreen const &render)
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

	RenderOffScreen &RenderOffScreen::pushSetUniformMat4Task(Key<Uniform> const &key, size_t location)
	{
		Task task;

		setTaskAllocation(task, (RenderOffScreen *)&_draw, Key<Uniform>(key), location);
		task.func = setUniformMat4byLocation;
		_tasks.push_back(task);
		return (*this);
	}

	RenderOffScreen &RenderOffScreen::pushSetUniformMat3Task(Key<Uniform> const &key, size_t location)
	{
		Task task;

		setTaskAllocation(task, (RenderOffScreen *)&_draw, Key<Uniform>(key), location);
		task.func = setUniformMat3byLocation;
		_tasks.push_back(task);
		return (*this);
	}

	RenderOffScreen &RenderOffScreen::pushSetUniformFloatTask(Key<Uniform> const &key, size_t location)
	{
		Task task;

		setTaskAllocation(task, (RenderOffScreen *)&_draw, Key<Uniform>(key), location);
		task.func = setUniformFloatbyLocation;
		_tasks.push_back(task);
		return (*this);
	}

	RenderOffScreen &RenderOffScreen::pushSetUniformVec4Task(Key<Uniform> const &key, size_t location)
	{
		Task task;

		setTaskAllocation(task, (RenderOffScreen *)&_draw, Key<Uniform>(key), location);
		task.func = setUniformVec4byLocation;
		_tasks.push_back(task);
		return (*this);
	}

	RenderOffScreen &RenderOffScreen::pushOwnTask(std::function<void (LocationStorage &)> const &f)
	{
		Task task;

		_ownFunction.push_back(new std::function<void(LocationStorage &)>(f));
		setTaskAllocation(task, _ownFunction.back(), &((RenderOffScreen::Draw *)&_draw)->locationStorage);
		task.func = ownTask;
		_tasks.push_back(task);
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

	RenderPass::RenderPass(Shader &s, GeometryManager &g, MaterialManager &m, LocationStorage &l)
		: RenderOffScreen(new Draw(g, l, s, m, GL_TRIANGLES)),
		_draw((Draw &)Render::_draw)
	{
	}

	RenderPass::~RenderPass()
	{
	}

	RenderPass &RenderPass::pushDrawTask()
	{
		Task task;

		setTaskAllocation(task, (RenderPass::Draw *)&_draw);
		task.func = draw;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::setDraw(AGE::Vector<AGE::Drawable> const &objects, size_t start, size_t end)
	{
		_draw.toRender = &objects;
		_draw.start = start;
		_draw.end = end;
		return (*this);
	}

	RenderPass &RenderPass::setDraw()
	{
		_draw.toRender = NULL;
		_draw.start = 0;
		_draw.end = 0;
		return (*this);
	}

	Render &RenderPass::render()
	{
		_fbo.bind();
		updateOutput();
		updateInput();
		_draw.shader.use();
		for (size_t i = 0; i < _tasks.size(); ++i)
			_tasks[i].func(_tasks[i].params);
		glFlush();
		return (*this);
	}


	RenderPostEffect::RenderPostEffect(Key<Vertices> const &key, Key<Indices> const &id, Shader &s, GeometryManager &g, LocationStorage &l)
		: RenderOffScreen(new Draw(g, l, s, GL_TRIANGLES, key, id)),
		_draw((Draw &)Render::_draw)
	{

	}

	RenderPostEffect::~RenderPostEffect()
	{

	}

	Render &RenderPostEffect::render()
	{
		_fbo.bind();
		updateOutput();
		updateInput();
		_draw.shader.use();
		for (size_t index = 0; index < _tasks.size(); ++index)
			_tasks[index].func(_tasks[index].params);
		_draw.shader.update();
		_draw.geometryManager.draw(_draw.mode, _draw.idQuad, _draw.quad);
		glFlush();
		return (*this);
	}

	RenderOnScreen::RenderOnScreen(Key<Vertices> const &key, Key<Indices> const &id, Shader &s, GeometryManager &g)
		: Render(new Draw(g, s, GL_TRIANGLES, key, id)),
		_draw((Draw &)Render::_draw)
	{

	}

	RenderOnScreen::~RenderOnScreen()
	{

	}

	Render &RenderOnScreen::render()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK);
		updateInput();
		_draw.shader.use();
		for (size_t index = 0; index < _tasks.size(); ++index)
			_tasks[index].func(_tasks[index].params);
		_draw.shader.update();
		_draw.geometryManager.draw(_draw.mode, _draw.quadId, _draw.quad);
		glFlush();
		return (*this);
	}

	RenderOnScreen::Draw::Draw(GeometryManager &g, Shader &s, GLenum mode, Key<Vertices> const &quad, Key<Indices> const &id)
		: Render::Draw(g, s, mode),
		quad(quad),
		quadId(id)
	{
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

	Render::Draw::Draw(GeometryManager &g, Shader &s, GLenum mode)
		: geometryManager(g),
		shader(s),
		mode(mode)
	{
	}

	Render::Input::Input(Key<Sampler> const &sampler, GLenum attachement, RenderOffScreen const &render)
		: sampler(sampler),
		attachement(attachement),
		render(render)
	{

	}

	Render::Input::Input(Input const &copy)
		: sampler(copy.sampler),
		attachement(copy.attachement),
		render(copy.render)
	{

	}
	
	Render::Input::~Input()
	{

	}

	RenderPass::Draw::Draw(GeometryManager &g, LocationStorage &l, Shader &s, MaterialManager &m, GLenum mode)
		: RenderOffScreen::Draw(g, l, s, mode),
		materialManager(m),
		toRender(NULL),
		start(0),
		end(0)
	{
	}

	RenderPostEffect::Draw::Draw(GeometryManager &g, LocationStorage &l, Shader &s, GLenum mode, Key<Vertices> const &quad, Key<Indices> const &id) 
		: RenderOffScreen::Draw(g, l, s, mode),
		quad(quad),
		idQuad(id)
	{
	}

	RenderOffScreen::Draw::Draw(GeometryManager &g, LocationStorage &l, Shader &s, GLenum mode)
		: Render::Draw(g, s, mode),
		locationStorage(l)
	{
	}

}