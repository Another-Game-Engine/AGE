#include <OpenGL/PixelBuffer.hh>
#include <utility>


namespace OpenGLTools
{
	PixelBuffer::PixelBuffer()
		: _memorySize(0)
	{
		glGenBuffers(1, &_id);
	}

	PixelBuffer::~PixelBuffer()
	{
		glDeleteBuffers(1, &_id);
	}

	PixelBuffer::PixelBuffer(PixelBuffer &&move)
		: _id(std::move(move._id)),
		_memorySize(std::move(move._memorySize))
	{
	}

	PixelBuffer &PixelBuffer::operator=(PixelBuffer &&p)
	{
		_id = std::move(p._id);
		_memorySize = p._memorySize;
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

	PixelBuffer const &PixelBufferPack::bind() const
	{
		glBindBuffer(GL_PIXEL_PACK_BUFFER, _id);
		return (*this);
	}

	PixelBuffer const &PixelBufferPack::unbind() const
	{
		glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
		return (*this);
	}

	PixelBuffer const &PixelBufferPack::allocate(std::uint32_t size)
	{
		if (_memorySize != size)
		{
			bind();
			glBufferData(GL_PIXEL_PACK_BUFFER, size, NULL, GL_STREAM_READ);
			_memorySize = size;
		}
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

	PixelBuffer const &PixelBufferUnPack::bind() const
	{
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, _id);
		return (*this);
	}

	PixelBuffer const &PixelBufferUnPack::unbind() const
	{
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		return (*this);
	}

	PixelBuffer const &PixelBufferUnPack::allocate(std::uint32_t size)
	{
		if (_memorySize != size)
		{
			bind();
			glBufferData(GL_PIXEL_UNPACK_BUFFER, size, NULL, GL_STREAM_READ);
			_memorySize = size;
		}
		return (*this);
	}
}
