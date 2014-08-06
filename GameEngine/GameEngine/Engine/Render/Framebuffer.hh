#pragma once

#include <Utils/OpenGL.hh>
#include <glm/glm.hpp>

namespace gl
{
	class Storage;

	class Framebuffer
	{
	public:
		Framebuffer(GLenum mode = GL_FRAMEBUFFER);
		~Framebuffer();

		// bind
		Framebuffer const &viewPort(glm::ivec4 const &rect) const;
		Framebuffer const &size(GLint width, GLint height, GLint sample = 1) const;
		Framebuffer const &bind() const;
		Framebuffer const &unbind() const;
		Framebuffer const &attachement(Storage const &storage, GLenum attach) const;
		GLuint getId() const;
		GLenum getType() const;

	private:
		GLuint _id;
		GLenum _mode;
	};
}
