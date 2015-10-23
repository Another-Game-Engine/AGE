#pragma once

#include <Utils/Debug.hpp>
#include <Utils/Profiler.hpp>
#include <Utils/OpenGL.hh>

#include "TextureBuffer.hh"

namespace AGE
{
	TextureBuffer::TextureBuffer()
	{}
	TextureBuffer::~TextureBuffer()
	{
		if (_textureHandle != -1)
		{
			glDeleteTextures(1, &_textureHandle);
		}
		if (_bufferHandle != -1)
		{
			glDeleteBuffers(1, &_bufferHandle);
		}
		if (_buffer)
		{
			free(_buffer);
		}
	}

	bool TextureBuffer::init(GLsizeiptr count, GLenum internal_format /*GL_R32F */, GLsizeiptr size, GLenum usage /* GL_STATIC_DRAW ...*/)
	{
		SCOPE_profile_cpu_function("TextureBuffer");
		AGE_ASSERT(_bufferHandle == -1 && _textureHandle == -1 && _count == 0 && _size == 0 && _buffer == nullptr);
		_count = count;
		_size = size;

		glGenBuffers(1, &_bufferHandle);
		bindBuffer();
		glBufferData(GL_TEXTURE_BUFFER, size * count, nullptr, usage);
		glGenTextures(1, &_textureHandle);
		bind();
		glTexBuffer(GL_TEXTURE_BUFFER, internal_format, _bufferHandle);
		_buffer = (char*)malloc(size * count);
		return true;
	}

	void TextureBuffer::resetOffset()
	{
		_offset = 0;
	}

	bool TextureBuffer::isFull() const
	{
		return _offset == _count;
	}

	bool TextureBuffer::isEmpty() const
	{
		return _offset == 0;
	}

	std::size_t TextureBuffer::getOffset() const
	{
		return _offset;
	}

	void TextureBuffer::push(GLvoid const *data)
	{
		memcpy(&_buffer[_offset * _size], data, _size);
		++_offset;
		AGE_ASSERT(_offset <= _count);
	}

	void TextureBuffer::sendBuffer()
	{
		SCOPE_profile_cpu_function("TextureBuffer");
		bindBuffer();
		glBufferSubData(GL_TEXTURE_BUFFER, 0, _offset * _size, _buffer);
	}

	void TextureBuffer::set(GLvoid const *data, GLsizeiptr count)
	{
		SCOPE_profile_cpu_function("TextureBuffer");
		AGE_ASSERT(count <= _count);
		bindBuffer();
		glBufferSubData(GL_TEXTURE_BUFFER, 0, _size * count, data);
	}

	void TextureBuffer::bindBuffer()
	{
		AGE_ASSERT(_bufferHandle != -1);
		glBindBuffer(GL_TEXTURE_BUFFER, _bufferHandle);
	}
	void TextureBuffer::unbindBuffer()
	{
		AGE_ASSERT(_bufferHandle != -1);
		glBindBuffer(GL_TEXTURE_BUFFER, 0);
	}
	void TextureBuffer::bind()
	{
		AGE_ASSERT(_textureHandle != -1);
		glBindTexture(GL_TEXTURE_BUFFER, _textureHandle);
	}
	void TextureBuffer::unbind()
	{
		AGE_ASSERT(_bufferHandle != -1);
		glBindTexture(GL_TEXTURE_BUFFER, 0);
	}
}