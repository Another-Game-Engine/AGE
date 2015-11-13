#pragma once

namespace AGE
{
	class TextureBuffer
	{
	public:
		TextureBuffer();
		~TextureBuffer();
		TextureBuffer(const TextureBuffer &) = delete;
		TextureBuffer(TextureBuffer &&) = delete;
		TextureBuffer &operator=(const TextureBuffer &) = delete;
		bool init(GLsizeiptr count, GLenum internal_format /*GL_R32F */, GLsizeiptr size, GLenum usage /* GL_STATIC_DRAW ...*/);
		void resetOffset();
		bool isFull() const;
		bool isEmpty() const;
		std::size_t getOffset() const;
		void push(GLvoid const *data);
		void sendBuffer();
		void set(GLvoid const *data, GLsizeiptr count);
		void bindBuffer();
		void unbindBuffer();
		void bind();
		void unbind();
	private:
		GLuint _bufferHandle = -1;
		GLuint _textureHandle = -1;
		GLsizeiptr _count = 0;
		GLsizeiptr _size = 0;
		GLsizeiptr _offset = 0;
		char* _buffer = nullptr;
	};
}