# pragma once

#include <Utils/OpenGL.hh>
#include <map>
#include <glm/glm.hpp>

namespace OpenGLTools
{
	class Texture;

	class Framebuffer
	{
	private:
		GLuint						_id;
		std::map<GLenum, Texture *>	_attachments;
		glm::uvec2					_size;
		int							_sampleNbr;
		bool						_multiSample;

	public:
		Framebuffer();
		~Framebuffer();

		Framebuffer(const Framebuffer &o)
		{
			_id = o._id;
			_attachments = o._attachments;
			_size = o._size;
			_sampleNbr = o._sampleNbr;
			_multiSample = o._multiSample;
		}

		Framebuffer &operator=(const Framebuffer &o)
		{
			_id = o._id;
			_attachments = o._attachments;
			_size = o._size;
			_sampleNbr = o._sampleNbr;
			_multiSample = o._multiSample;
			return *this;
		}

		void		init(glm::uvec2 size, int sampleNbr);

		void addTextureAttachment(GLenum textureInternalFormat, GLenum textureFormat, GLenum attachment);
		void attachAll() const;
		void bind() const;
		Texture *operator[](GLenum attachment) const;
		glm::uvec2	getSize();

		bool isInit() const { return (_id != 0); }
		bool isMultisampled() const { return (_multiSample); }
		int	getSampleNbr() const { return (_sampleNbr); }
		GLuint getId() const { return (_id); }
	private:
	};
}