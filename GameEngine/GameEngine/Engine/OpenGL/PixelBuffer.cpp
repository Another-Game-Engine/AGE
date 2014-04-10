#include "PixelBuffer.hh"

namespace OpenGLTools
{
	PixelBuffer::PixelBuffer()
		: _buffer1(0, false),
		_buffer2(0, false)
	{
		glGenBuffers(1, &_buffer1.first);
		glGenBuffers(1, &_buffer2.first);
	}

	PixelBuffer::~PixelBuffer()
	{
		glDeleteBuffers(1, &_buffer1.first);
		glDeleteBuffers(1, &_buffer2.first);
	}

	PixelBuffer::PixelBuffer(PixelBuffer const &copy)
		: _buffer1(copy._buffer1),
		_buffer2(copy._buffer2)
	{

	}

	PixelBuffer &PixelBuffer::operator=(PixelBuffer const &other)
	{
		_buffer1 = other._buffer1;
		_buffer2 = other._buffer2;
		return (*this);
	}

	void *PixelBuffer::getBuffer(glm::vec4 &&posDimension, GLenum format, GLenum component, GLenum buffer)
	{
		if (_buffer1.second == false && _buffer2.second == false)
		{
			_buffer1.second = true;
			glBindBuffer(GL_PIXEL_PACK_BUFFER, _buffer1.first);
			return (NULL);
		}
		else if (_buffer1.second == true)
		{
			glBindBuffer(GL_PIXEL_PACK_BUFFER, _buffer2.first);
			glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
			_buffer1.second = false;
			_buffer2.second = true;
			glReadBuffer(buffer);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, _buffer2.first);
	//		glReadPixels(posDimension[0], posDimension[1], posDimension[2], posDimension[3], format, component, 0);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, _buffer1.first);
			return (glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY));
		}
		else if (_buffer2.second == true)
		{
			glBindBuffer(GL_PIXEL_PACK_BUFFER, _buffer1.first);
			glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
			_buffer1.second = true;
			_buffer2.second = false;
			glBindBuffer(GL_PIXEL_PACK_BUFFER, _buffer1.first);
//			glReadPixels(posDimension[0], posDimension[1], posDimension[2], posDimension[3], format, component, 0);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, _buffer2.first);
			return (glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY));
		}
	}
}