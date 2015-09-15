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
		}
		TextureBuffer(const TextureBuffer &) = delete;
		TextureBuffer(TextureBuffer &&) = delete;
		TextureBuffer &operator=(const TextureBuffer &) = delete;

		bool init(GLsizeiptr size, GLenum internal_format /*GL_R32F */, GLenum usage /* GL_STATIC_DRAW ...*/)
		{
			AGE_ASSERT(_bufferHandle == -1 && _textureHandle == -1 && _size == 0);
			_size = size;

			glGenBuffers(1, &_bufferHandle);
			bindBuffer();
			glBufferData(GL_TEXTURE_BUFFER, size, nullptr, usage);
			glGenTextures(1, &_textureHandle);
			bind();
			glTexBuffer(GL_TEXTURE_BUFFER, internal_format, _bufferHandle);
		}
		void set(GLvoid const *data, GLsizeiptr size)
		{
			AGE_ASSERT(size <= _size);
			bindBuffer();
			glBufferSubData(GL_TEXTURE_BUFFER, 0, size, data);
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
		GLsizeiptr _size = 0;
	};
}