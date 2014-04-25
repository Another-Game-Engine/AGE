#include <OpenGL/PixelBuffer.hh>
#include <utility>


namespace OpenGLTools
{
	PixelBuffer::PixelBuffer()
	{
		glGenBuffers(1, &_id);
	}

	PixelBuffer::~PixelBuffer()
	{
		glDeleteBuffers(1, &_id);
	}

	PixelBuffer::PixelBuffer(PixelBuffer &&move)
		: _id(std::move(move._id))
	{
	}

	PixelBuffer &PixelBuffer::operator=(PixelBuffer &&p)
	{
		_id = std::move(p._id);
		return (*this);
	}

	GLint PixelBuffer::getId() const
	{
		return (_id);
	}

	PixelBufferPack::PixelBufferPack()
		: PixelBuffer()
	{
	}

	PixelBufferPack::~PixelBufferPack()
	{
	}

	PixelBufferPack::PixelBufferPack(PixelBufferPack &&move)
		: PixelBuffer(std::move(move))
	{
	}

	PixelBuffer &PixelBufferPack::bind()
	{
		glBindBuffer(GL_PIXEL_PACK_BUFFER, _id);
		return (*this);
	}

	PixelBuffer &PixelBufferPack::unbind()
	{
		glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
		return (*this);
	}

	PixelBufferUnPack::PixelBufferUnPack()
		: PixelBuffer()
	{
	}

	PixelBufferUnPack::~PixelBufferUnPack()
	{
	}

	PixelBufferUnPack::PixelBufferUnPack(PixelBufferUnPack &&move)
		: PixelBuffer(std::move(move))
	{
	}

	PixelBuffer &PixelBufferUnPack::bind()
	{
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, _id);
		return (*this);
	}

	PixelBuffer &PixelBufferUnPack::unbind()
	{
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		return (*this);
	}
}
