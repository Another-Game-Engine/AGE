#include <Render/OpenGLTask/OpenGLState.hh>

namespace AGE
{
	OpenGLState::State OpenGLState::_currentState;

	const OpenGLState::ToStateMode OpenGLState::_glToStateMode[State::NUMBER_OF_MODES] =
	{
		{ GL_ALPHA_TEST, State::ALPHA_TEST },
		{ GL_BLEND, State::BLEND },
		{ GL_CULL_FACE, State::CULL_FACE },
		{ GL_DEPTH_TEST, State::DEPTH_TEST },
		{ GL_STENCIL_TEST, State::STENCIL_TEST },
		{ GL_SCISSOR_TEST, State::SCISSOR_TEST },
		{ GL_TEXTURE_1D, State::TEXTURE_1D },
		{ GL_TEXTURE_2D, State::TEXTURE_2D },
		{ GL_TEXTURE_3D, State::TEXTURE_3D },
		{ GL_TEXTURE_CUBE_MAP, State::TEXTURE_CUBE_MAP }
	};

	OpenGLState::State::State()
	{
		depth.func = GL_LESS;
		depth.mask = GL_TRUE;
		stencil.func = GL_ALWAYS;
		stencil.ref = 0;
		stencil.mask = 0xFFFFFFFF;
		stencil.op.sfail = GL_KEEP;
		stencil.op.dpfail = GL_KEEP;
		stencil.op.dppass = GL_KEEP;
		blend.sfactor = GL_ONE;
		blend.dfactor = GL_ZERO;
		blend.equation = GL_FUNC_ADD;
		cullFace = GL_BACK;
		colorMask = glm::bvec4(true);
		clearColor = glm::vec4(0);
		clearDepth = 1.0;
		clearStencil = 0;
	}

	void OpenGLState::glEnable(GLenum cap)
	{
		State::Modes mode = _findMode(cap);

		assert(mode != State::NUMBER_OF_MODES);
		if (_currentState.enabledModes[mode] == false)
		{
			::glEnable(cap);
			_currentState.enabledModes[mode] = true;
		}
	}

	void OpenGLState::glDisable(GLenum cap)
	{
		State::Modes mode = _findMode(cap);

		assert(mode != State::NUMBER_OF_MODES);
		if (_currentState.enabledModes[mode] == true)
		{
			::glDisable(cap);
			_currentState.enabledModes[mode] = false;
		}
	}

	void OpenGLState::glDepthFunc(GLenum func)
	{
		if (_currentState.depth.func != func)
		{
			::glDepthFunc(func);
			_currentState.depth.func = func;
		}
	}

	void OpenGLState::glDepthMask(GLenum flag)
	{
		if (_currentState.depth.mask != flag)
		{
			::glDepthMask(flag);
			_currentState.depth.mask = flag;
		}
	}

	void OpenGLState::glStencilFunc(GLenum func, GLint ref, GLuint mask)
	{
		if (_currentState.stencil.func != func ||
			_currentState.stencil.ref != ref || 
			_currentState.stencil.mask != mask)
		{
			::glStencilFunc(func, ref, mask);
			_currentState.stencil.func = func;
			_currentState.stencil.ref = ref;
			_currentState.stencil.mask = mask;
		}
	}

	void OpenGLState::glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
	{
		if (_currentState.stencil.op.sfail != sfail ||
			_currentState.stencil.op.dpfail != dpfail ||
			_currentState.stencil.op.dppass != dppass)
		{
			::glStencilOp(sfail, dpfail, dppass);
			_currentState.stencil.op.sfail = sfail;
			_currentState.stencil.op.dpfail = dpfail;
			_currentState.stencil.op.dppass = dppass;
		}
	}

	void OpenGLState::glBlendFunc(GLenum sfactor, GLenum dfactor)
	{
		if (_currentState.blend.sfactor != sfactor ||
			_currentState.blend.dfactor != dfactor)
		{
			::glBlendFunc(sfactor, dfactor);
			_currentState.blend.sfactor = sfactor;
			_currentState.blend.dfactor = dfactor;
		}
	}

	void OpenGLState::glBlendEquation(GLenum mode)
	{
		if (_currentState.blend.equation != mode)
		{
			::glBlendEquation(mode);
			_currentState.blend.equation = mode;
		}
	}

	void OpenGLState::glCullFace(GLenum mode)
	{
		if (_currentState.cullFace != mode)
		{
			::glCullFace(mode);
			_currentState.cullFace = mode;
		}
	}

	void OpenGLState::glColorMask(glm::bvec4 const &mask)
	{
		if (_currentState.colorMask != mask)
		{
			::glColorMask(mask.r, mask.g, mask.b, mask.a);
			_currentState.colorMask = mask;
		}
	}

	void OpenGLState::glClearColor(glm::vec4 const &ref)
	{
		if (_currentState.clearColor != ref)
		{
			::glClearColor(ref.r, ref.g, ref.b, ref.a);
			_currentState.clearColor = ref;
		}
	}

	void OpenGLState::glClearDepth(GLclampd ref)
	{
		if (_currentState.clearDepth != ref)
		{
			::glClearDepth(ref);
			_currentState.clearDepth = ref;
		}
	}

	void OpenGLState::glClearStencil(GLint ref)
	{
		if (_currentState.clearStencil != ref)
		{
			::glClearStencil(ref);
			_currentState.clearStencil = ref;
		}
	}

	OpenGLState::State::Modes OpenGLState::_findMode(GLenum ogl)
	{
		for (int i = 0; i < State::NUMBER_OF_MODES; ++i)
		{
			if (_glToStateMode[i].ogl == ogl)
				return (_glToStateMode[i].mode);
		}
		return (State::NUMBER_OF_MODES);
	}

	void OpenGLState::setCurrentState(State const &toSet)
	{
		for (int i = 0; i < State::NUMBER_OF_MODES; ++i)
		{
			if (_currentState.enabledModes[i] != toSet.enabledModes[i])
			{
				::glEnable(_glToStateMode[i].ogl);
			}
		}
		_currentState.enabledModes = toSet.enabledModes;
		glDepthFunc(toSet.depth.func);
		glDepthMask(toSet.depth.mask);
		glStencilFunc(toSet.stencil.func, toSet.stencil.ref, toSet.stencil.mask);
		glStencilOp(toSet.stencil.op.sfail, toSet.stencil.op.dpfail, toSet.stencil.op.dppass);
		glBlendFunc(toSet.blend.sfactor, toSet.blend.dfactor);
		glBlendEquation(toSet.blend.equation);
		glCullFace(toSet.cullFace);
		glClearColor(toSet.clearColor);
		glClearDepth(toSet.clearDepth);
		glClearStencil(toSet.clearStencil);
	}


}
