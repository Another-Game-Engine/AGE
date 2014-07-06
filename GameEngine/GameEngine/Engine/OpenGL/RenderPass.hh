#pragma once

#include <Utils/OpenGL.hh>

namespace gl
{
	//!\file RenderPass.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class RenderPass
	//!\brief Handle one of opengl render pass
	class RenderPass
	{
	public:
		RenderPass();
		~RenderPass();
		RenderPass(RenderPass const &copy);
		RenderPass &operator=(RenderPass const &r);

		RenderPass &setScissorTest(bool state);
		RenderPass &setMultiSampling(bool state, float ratio);

		RenderPass &setScissor(GLint left, GLint bottom, GLsizei width, GLsizei height);
		RenderPass &setColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
		RenderPass &setDepth(GLclampd depth);
		RenderPass &setStencil(GLint stencil);
		RenderPass &setMaskColor(GLuint index, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
		RenderPass &setMaskDepth(GLboolean depth);
		RenderPass &setStencilMaskBackFace(GLboolean state);
		RenderPass &setStencilMaskFrontFace(GLboolean state);
		RenderPass &setClearColor(bool state);
		RenderPass &setClearDepth(bool state);
		RenderPass &setClearStencil(bool state);
		RenderPass &use();

	private:
		GLbitfield _clearColor;
		GLbitfield _clearDepth;
		GLbitfield _clearStencil;
		bool _scissorTest;
		bool _multisampling;
		float _multisampleRatio;
		GLint _leftScissor;
		GLint _bottomScissor;
		GLsizei _widthScissor;
		GLsizei _heightScissor;
		GLclampf _red;
		GLclampf _green;
		GLclampf _blue;
		GLclampf _alpha;
		GLclampd _depth;
		GLint _stencil;
		GLboolean _maskRed[GL_MAX_COLOR_ATTACHMENTS];
		GLboolean _maskGreen[GL_MAX_COLOR_ATTACHMENTS];
		GLboolean _maskBlue[GL_MAX_COLOR_ATTACHMENTS];
		GLboolean _maskAlpha[GL_MAX_COLOR_ATTACHMENTS];
		GLboolean _maskDepth;
		GLint _maskStencilBack;
		GLint _maskStencilFront;



	};
}