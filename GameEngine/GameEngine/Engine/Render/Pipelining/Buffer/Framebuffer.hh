#pragma once

#include <Utils/OpenGL.hh>

namespace AGE
{

	class IFramebufferStorage;

	class Framebuffer
	{
	public:
		Framebuffer(GLenum mode = GL_FRAMEBUFFER);
		~Framebuffer();

	public:
		Framebuffer const &size(GLint width, GLint height, GLint sample = 1) const;
		Framebuffer const &bind() const;
		Framebuffer const &unbind() const;
		Framebuffer const &attachment(IFramebufferStorage const &storage, GLenum attach) const;
		GLuint id() const;
		GLenum type() const;

	private:
		GLuint _id;
		GLenum _mode;
	};

}