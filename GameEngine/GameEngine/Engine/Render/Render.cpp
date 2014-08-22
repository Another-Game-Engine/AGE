#include <Render/Render.hh>
#include <cassert>
#include <string>
#include <Core/Drawable.hh>
#include <Render/OpenGLTask.hh>
#include <Render/GeometryManager.hh>
#include <Render/MaterialManager.hh>
#include <Render/Storage.hh>

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

	Render &Render::pushInputSampler(Key<Sampler> const &key)
	{
		if (_shader.hasSampler(key))
			_inputSamplers.push_back(key);
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
		if (_branch != NULL && _inputSamplers.size() == _branch->getNbrAttachementOutput())
		{
			for (size_t index = 0; index < _inputSamplers.size(); ++index)
				_shader.setSampler(_inputSamplers[index], _branch->getColorOutput(index));
		}
	}
	
	RenderOffScreen &RenderOffScreen::useInputDepth()
	{
		_useInputDepth = true;
		return (*this);
	}

	RenderOffScreen &RenderOffScreen::unUseInputDepth()
	{
		_useInputDepth = false;
		return (*this);
	}

	RenderOffScreen &RenderOffScreen::useInputStencil()
	{
		_useInputStencil = true;
		return (*this);
	}

	RenderOffScreen &RenderOffScreen::unUseInputStencil()
	{
		_useInputStencil = false;
		return (*this);
	}

	RenderOffScreen::RenderOffScreen(Shader &shader, GeometryManager &g)
		: Render(shader, g),
		_sample(1),
		_colorAttachement(NULL),
		_colorTexture2D(NULL),
		_nbrColorAttachement(0),
		_depthBuffer(NULL),
		_stencilBuffer(NULL),
		_useInputDepth(false),
		_useInputStencil(false),
		_updateOutput(false)
	{
	}

	RenderOffScreen::~RenderOffScreen()
	{
		if (_nbrColorAttachement > 0)
		{
			delete[] _colorAttachement;
			delete[] _colorTexture2D;
		}
		if (_depthBuffer != NULL)
			delete _depthBuffer;
		if (_stencilBuffer)
			delete _stencilBuffer;
	}

	RenderOffScreen &RenderOffScreen::configSample(GLint sample)
	{
		_sample = sample;
		return (*this);
	}

	RenderOffScreen &RenderOffScreen::pushColorOutput(GLenum attachement, GLenum internalFormat)
	{
		return (pushColorOutput(attachement, _rect.z, _rect.w, internalFormat));
	}

	RenderOffScreen &RenderOffScreen::pushColorOutput(GLenum attachement, size_t width, size_t height, GLenum internalFormat)
	{
		for (uint8_t index = 0; index < _nbrColorAttachement; ++index)
		{
			if (attachement == _colorAttachement[index])
				return (*this);
		}
		GLsizei tmp_nbrColorAttachement = _nbrColorAttachement + 1;
		GLenum *tmp_colorAttachement = new GLenum[tmp_nbrColorAttachement];
		Texture2D **tmp_colorTexture2D = new Texture2D *[tmp_nbrColorAttachement];
		memcpy(tmp_colorAttachement, _colorAttachement, sizeof(GLenum)* _nbrColorAttachement);
		memcpy(tmp_colorTexture2D, _colorTexture2D, sizeof(GLenum)* _nbrColorAttachement);
		if (_nbrColorAttachement > 0)
		{
			delete[] _colorAttachement;
			delete[] _colorTexture2D;
		}
		_colorAttachement = tmp_colorAttachement;
		_colorTexture2D = tmp_colorTexture2D;
		_nbrColorAttachement = tmp_nbrColorAttachement;
		_colorTexture2D[_nbrColorAttachement - 1] = new Texture2D(width, height, internalFormat, false);
		_colorAttachement[_nbrColorAttachement - 1] = attachement;
		_updateOutput = true;
		return (*this);
	}

	RenderOffScreen &RenderOffScreen::popColorOutput()
	{
		if (_nbrColorAttachement < 1)
			return (*this);
		GLsizei tmp_nbrColorAttachement = _nbrColorAttachement - 1;
		GLenum *tmp_colorAttachement = new GLenum[tmp_nbrColorAttachement];
		Texture2D **tmp_colorTexture2D = new Texture2D *[tmp_nbrColorAttachement];
		memcpy(tmp_colorAttachement, _colorAttachement, sizeof(GLenum)* tmp_nbrColorAttachement);
		memcpy(tmp_colorTexture2D, _colorTexture2D, sizeof(GLenum)* tmp_nbrColorAttachement);
		delete[] _colorAttachement;
		delete[] _colorTexture2D;
		_colorAttachement = tmp_colorAttachement;
		_colorTexture2D = tmp_colorTexture2D;
		_updateOutput = true;
		return (*this);
	}

	Texture2D const &RenderOffScreen::getColorOutput(size_t index) const
	{
		return (*_colorTexture2D[index]);
	}

	GLenum RenderOffScreen::getAttachementOutput(size_t index)
	{
		return (_colorAttachement[index]);
	}

	size_t RenderOffScreen::getNbrAttachementOutput() const
	{
		return (_nbrColorAttachement);
	}

	RenderOffScreen &RenderOffScreen::createDepthBuffer()
	{
		if (_depthBuffer != NULL)
			return (*this);
		_depthBuffer = new RenderBuffer(_rect.z, _rect.w, GL_DEPTH_COMPONENT);
		return (*this);
	}

	RenderOffScreen &RenderOffScreen::deleteDepthBuffer()
	{
		if (_depthBuffer == NULL)
			return (*this);
		delete _depthBuffer;
		_depthBuffer = NULL;
		return (*this);
	}

	RenderBuffer const *RenderOffScreen::getDepthBuffer() const
	{
		return (_depthBuffer);
	}

	RenderOffScreen &RenderOffScreen::createStencilBuffer()
	{
		if (_stencilBuffer != NULL)
			return (*this);
		_stencilBuffer = new RenderBuffer(_rect.z, _rect.w, GL_DEPTH_COMPONENT);
		return (*this);
	}

	RenderOffScreen &RenderOffScreen::deleteStencilBuffer()
	{
		if (_stencilBuffer == NULL)
			return (*this);
		delete _stencilBuffer;
		_stencilBuffer = NULL;
		return (*this);
	}

	void RenderOffScreen::updateOutput()
	{
		glDrawBuffers(_nbrColorAttachement, _colorAttachement);
		for (size_t index = 0; index < _nbrColorAttachement; ++index)
			_fbo.attachement(*_colorTexture2D[index], _colorAttachement[index]);
		if (_depthBuffer != NULL && _useInputDepth == false)
			_fbo.attachement(*_depthBuffer, GL_DEPTH_ATTACHMENT);
		if (_stencilBuffer != NULL && _useInputStencil == false)
			_fbo.attachement(*_stencilBuffer, GL_STENCIL_ATTACHMENT);
		_updateOutput = false;
	}

	void RenderOffScreen::updateInput()
	{
		Render::updateInput();
		if (_useInputDepth && _branch->getDepthBuffer() != NULL)
			_fbo.attachement(*_branch->getDepthBuffer(), GL_DEPTH_ATTACHMENT);
		if (_useInputStencil && _branch->getStencilBuffer() != NULL)
			_fbo.attachement(*_branch->getStencilBuffer(), GL_STENCIL_ATTACHMENT);
	}

	RenderBuffer const *RenderOffScreen::getStencilBuffer() const
	{
		return (_stencilBuffer);
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
		if (_updateOutput)
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
		if (_updateOutput)
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
}