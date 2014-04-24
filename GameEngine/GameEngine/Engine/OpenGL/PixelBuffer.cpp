#include <OpenGL/PixelBuffer.hh>
#include <utility>


namespace OpenGLTools
{
	PixelBuffer::PixelBuffer()
		: _current(0)
	{
	}

	PixelBuffer::~PixelBuffer()
	{
	}

	PixelBuffer::PixelBuffer(PixelBuffer const &copy)
		: _current(copy._current)
	{
		_id[0] = copy._id[0];
		_id[1] = copy._id[1];
	}

	PixelBuffer::PixelBuffer(PixelBuffer &&move)
		: _current(std::move(move._current))
	{
		_id[0] = std::move(move._id[0]);
		_id[1] = std::move(move._id[1]);
	}

	PixelBuffer &PixelBuffer::operator=(PixelBuffer const &p)
	{
		_id[0] = p._id[0];
		_id[1] = p._id[1];
		_current = p._current;
		return (*this);
	}

	PixelBuffer &PixelBuffer::operator=(PixelBuffer &&p)
	{
		_id[0] = std::move(p._id[0]);
		_id[1] = std::move(p._id[1]);
		_current = std::move(_current);
		return (*this);
	}

	PixelBufferPack::PixelBufferPack()
		: PixelBuffer()
	{
	}

	PixelBufferPack::~PixelBufferPack()
	{
	}

	PixelBufferPack::PixelBufferPack(PixelBufferPack const &copy)
		: PixelBuffer(copy)
	{
		glBindBuffer(GL_PIXEL_PACK_BUFFER, _id[0]);
	}

	PixelBufferPack::PixelBufferPack(PixelBufferPack &&move)
		: PixelBuffer(std::move(move))
	{
	}

	PixelBuffer &PixelBufferPack::init()
	{
		glGenBuffers(NBR_BUFFER, _id);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, _id[0]);
		glBufferData(GL_PIXEL_PACK_BUFFER, 0, 0, GL_STREAM_DRAW);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, _id[1]);
		glBufferData(GL_PIXEL_PACK_BUFFER, 0, 0, GL_STREAM_DRAW);
		return (*this);
	}

	void PixelBufferPack::unload() const
	{
		glDeleteBuffers(2, _id);
	}

	PixelBuffer &PixelBufferPack::bind()
	{
		glBindBuffer(GL_PIXEL_PACK_BUFFER, _id[1 - _current]);
		return (*this);
	}

	PixelBuffer &PixelBufferPack::unbind()
	{
		glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
		_current = 1 - _current;
		return (*this);
	}

	PixelBufferPack &PixelBufferPack::mapBuffer(void **data)
	{
		glBindBuffer(GL_PIXEL_PACK_BUFFER, _id[_current]);
		*data = static_cast<void *>(glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY));
		return (*this);
	}

	PixelBufferPack &PixelBufferPack::unmapBuffer(void **data)
	{
		if (data && *data)
			glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
		return (*this);
	}

	PixelBufferUnPack::PixelBufferUnPack()
		: PixelBuffer(),
		_size(0)
	{
	}

	PixelBufferUnPack::~PixelBufferUnPack()
	{
	}

	PixelBufferUnPack::PixelBufferUnPack(PixelBufferUnPack const &copy)
		: PixelBuffer(copy), 
		_size(copy._size)
	{
	}

	PixelBufferUnPack::PixelBufferUnPack(PixelBufferUnPack &&move)
		: PixelBuffer(std::move(move)),
		_size(std::move(move._size))
	{
	}

	PixelBuffer &PixelBufferUnPack::init()
	{
		glGenBuffers(NBR_BUFFER, _id);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, _id[0]);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, 0, 0, GL_STREAM_DRAW);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, _id[1]);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, 0, 0, GL_STREAM_DRAW);
		return (*this);
	}

	void PixelBufferUnPack::unload() const
	{
		glDeleteBuffers(2, _id);
	}

	PixelBuffer &PixelBufferUnPack::bind()
	{
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, _id[1 - _current]);
		return (*this);
	}

	PixelBuffer &PixelBufferUnPack::unbind()
	{
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		_current = 1 - _current;
		return (*this);
	}

	PixelBufferUnPack &PixelBufferUnPack::setBuffer(void *data, std::uint32_t size_data)
	{
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, _id[_current]);
		if (_size != size_data)
		{
			_size = size_data;
			glBufferData(GL_PIXEL_UNPACK_BUFFER, size_data, data, GL_STREAM_DRAW);
		}
		else
			glBufferSubData(GL_PIXEL_UNPACK_BUFFER, 0, size_data, data);
		return (*this);
	}
}
