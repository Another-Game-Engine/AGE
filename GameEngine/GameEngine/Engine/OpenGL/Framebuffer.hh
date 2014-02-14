#ifndef  __FRAMEBUFFER_HH__
# define __FRAMEBUFFER_HH__

#include <map>
#include <vector>
#include "Utils/OpenGL.hh"
#include "VertexArray.hh"

namespace OpenGLTools
{
	class Framebuffer
	{
	private:
		GLuint						_id;
		std::map<GLenum, GLuint>	_attachments;
		glm::uvec2					_size;

	public:
		Framebuffer();
		~Framebuffer();

		void		init(glm::uvec2 size);

		void		addTextureAttachment(GLenum textureInternalFormat, GLenum textureFormat, GLenum attachment);
		void		attachAll() const;
		void		bind() const;
		GLuint		getTextureAttachment(GLenum attachment) const;
		glm::uvec2	getSize();

		bool		isInit() const { return (_id != 0); }

	private:
		Framebuffer(const Framebuffer &o);
		Framebuffer &operator=(const Framebuffer &o);
	};
}

#endif   //!__FRAMEBUFFER_HH__