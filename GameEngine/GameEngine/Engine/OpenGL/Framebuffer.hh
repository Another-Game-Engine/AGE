#ifndef  __FRAMEBUFFER_HH__
# define __FRAMEBUFFER_HH__

#include <map>
#include <vector>
#include "Utils/OpenGL.hh"
#include "glm/glm.hpp"
#include "VertexArray.hh"

namespace OpenGLTools
{
	class Framebuffer
	{
	private:
		GLuint						_id;
		std::map<GLenum, GLuint>	_attachments;
		glm::uvec2					_size;
		int							_sampleNbr;
		bool						_multiSample;

	public:
		Framebuffer();
		~Framebuffer();

		void		init(glm::uvec2 size, int sampleNbr);

		void		addTextureAttachment(GLenum textureInternalFormat, GLenum textureFormat, GLenum attachment);
		void		attachAll() const;
		void		bind() const;
		GLuint		getTextureAttachment(GLenum attachment) const;
		glm::uvec2	getSize();

		bool		isInit() const { return (_id != 0); }
		bool		isMultisampled() const { return (_multiSample); }
		int			getSampleNbr() const { return (_sampleNbr); }
		GLuint		getId() const { return (_id); }
	private:
		Framebuffer(const Framebuffer &o);
		Framebuffer &operator=(const Framebuffer &o);
	};
}

#endif   //!__FRAMEBUFFER_HH__