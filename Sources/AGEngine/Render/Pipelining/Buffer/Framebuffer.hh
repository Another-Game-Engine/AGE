#pragma once

#include <Utils/OpenGL.hh>

namespace AGE
{

	class IFramebufferStorage;

	class Framebuffer
	{
	public:
		Framebuffer(GLint width, GLint height, GLint sample = 1, GLenum mode = GL_FRAMEBUFFER);
		Framebuffer(Framebuffer const &) = delete;
		Framebuffer(Framebuffer &&move);
		~Framebuffer();

	public:
		Framebuffer const &size(GLint width, GLint height, GLint sample = 1);
		Framebuffer const &bind() const;
		Framebuffer const &unbind() const;
		Framebuffer const &attachment(IFramebufferStorage const &storage, GLenum attach) const;
		GLuint id() const;
		GLenum type() const;
		inline GLint width() const { return _width; }
		inline GLint height() const { return _height; }

	private:
		GLuint _id;
		GLenum _mode;
		GLint _width;
		GLint _height;
		GLint _sample;
	};

}