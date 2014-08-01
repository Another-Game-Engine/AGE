#pragma once

#include <Utils/OpenGL.hh>

namespace gl
{
	class Framebuffer
	{
	public:
		Framebuffer(GLenum mode = GL_FRAMEBUFFER, GLint width = 512, GLint height = 512, GLint sampler = 1);
		~Framebuffer();

		// bind
		Framebuffer const &size(GLint width, GLint height, GLint sample = 1) const;
		Framebuffer const &bind() const;
		Framebuffer const &unbind() const;

	private:
		GLuint _id;
		GLenum _mode;
	};
}
