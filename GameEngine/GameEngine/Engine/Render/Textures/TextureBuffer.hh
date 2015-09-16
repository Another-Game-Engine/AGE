#pragma once

#include <Utils/Debug.hpp>

namespace AGE
{
	class TextureBuffer
	{
	public:
		TextureBuffer()
		{}
		~TextureBuffer()
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
		TextureBuffer(const TextureBuffer &) = delete;
		TextureBuffer(TextureBuffer &&) = delete;
		TextureBuffer &operator=(const TextureBuffer &) = delete;

		bool init(GLsizeiptr count, GLenum internal_format /*GL_R32F */, GLsizeiptr size, GLenum usage /* GL_STATIC_DRAW ...*/)
		{
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

		void resetOffset()
		{
			_offset = 0;
		}

		bool isFull() const
		{
			return _offset == _count;
		}

		bool isEmpty() const
		{
			return _offset == 0;
		}

		std::size_t getOffset() const
		{
			return _offset;
		}

		void push(GLvoid const *data)
		{
			memcpy(&_buffer[_offset * _size], data, _size);
			++_offset;
			AGE_ASSERT(_offset <= _count);
		}

		void sendBuffer()
		{
			bindBuffer();
			glBufferSubData(GL_TEXTURE_BUFFER, 0, _offset * _size, _buffer);
		}

		void set(GLvoid const *data, GLsizeiptr count)
		{
			AGE_ASSERT(count <= _count);
			bindBuffer();
			glBufferSubData(GL_TEXTURE_BUFFER, 0, _size * count, data);
		}
		inline void bindBuffer()
		{
			AGE_ASSERT(_bufferHandle != -1);
			glBindBuffer(GL_TEXTURE_BUFFER, _bufferHandle);
		}
		inline void unbindBuffer()
		{
			AGE_ASSERT(_bufferHandle != -1);
			glBindBuffer(GL_TEXTURE_BUFFER, 0);
		}
		inline void bind()
		{
			AGE_ASSERT(_textureHandle != -1);
			glBindTexture(GL_TEXTURE_BUFFER, _textureHandle);
		}
		inline void unbind()
		{
			AGE_ASSERT(_bufferHandle != -1);
			glBindTexture(GL_TEXTURE_BUFFER, 0);
		}
	private:
		GLuint _bufferHandle = -1;
		GLuint _textureHandle = -1;
		GLsizeiptr _count = 0;
		GLsizeiptr _size = 0;
		GLsizeiptr _offset = 0;
		char* _buffer = nullptr;
	};
}