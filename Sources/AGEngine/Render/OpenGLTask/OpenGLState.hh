#pragma once

#include <Utils/OpenGL.hh>

#include <glm/glm.hpp>
#include <bitset>

namespace AGE
{
	class OpenGLState
	{
	public:
		struct State
		{
			enum Modes
			{
				ALPHA_TEST = 0,
				BLEND,
				CULL_FACE,
				DEPTH_TEST,
				STENCIL_TEST,
				SCISSOR_TEST,
				TEXTURE_1D,
				TEXTURE_2D,
				TEXTURE_3D,
				TEXTURE_CUBE_MAP,
				NUMBER_OF_MODES
			};

			State();

			std::bitset<NUMBER_OF_MODES> enabledModes;
			
			struct Depth
			{
				GLenum func;
				GLenum mask;
			} depth;
			
			struct Stencil
			{
				GLenum func;
				GLint ref;
				GLuint mask;
				struct Op
				{
					GLenum sfail;
					GLenum dpfail;
					GLenum dppass;
				} op;
			} stencil;
			
			struct Blending
			{
				GLenum sfactor;
				GLenum dfactor;
				GLenum equation;
			} blend;
			
			GLenum cullFace;
			glm::bvec4 colorMask;
			glm::vec4 clearColor;
			GLclampd clearDepth;
			GLint clearStencil;
		};

	public:
		static void glEnable(GLenum cap);
		static void glDisable(GLenum cap);
		static void glDepthFunc(GLenum func);
		static void glDepthMask(GLenum flag);
		static void glStencilFunc(GLenum func, GLint ref, GLuint mask);
		static void glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
		static void glBlendFunc(GLenum sfactor, GLenum dfactor);
		static void glBlendEquation(GLenum mode);
		static void glCullFace(GLenum mode);
		static void glColorMask(glm::bvec4 const &mask);
		static void glClearColor(glm::vec4 const &ref);
		static void glClearDepth(GLclampd ref);
		static void glClearStencil(GLint ref);

		static void setCurrentState(State const &toSet);

	private:
		struct ToStateMode
		{
			const GLenum ogl;
			const State::Modes mode;
		};

		const static ToStateMode _glToStateMode[State::NUMBER_OF_MODES];
		
		static State::Modes _findMode(GLenum ogl);

	private:
		static State _currentState;
	};
}
