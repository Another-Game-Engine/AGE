#include "VertexBuffer.hh"

#include <assert.h>

namespace OpenGLTools
{
	VertexBuffer::VertexBuffer()
		: _size(0)
	{
	}

	 VertexBuffer::~VertexBuffer()
	 {
	 }

	 void VertexBuffer::init()
	 {
		 glGenBuffers(1, &_id);
	 }

	 void VertexBuffer::unload()
	 {
		 glDeleteBuffers(1, &_id);
	 }
	 
	 VertexBuffer::VertexBuffer(VertexBuffer const &copy)
		 : _id(copy._id),
		 _size(copy._size)
	 {

	 }

	 VertexBuffer &VertexBuffer::operator=(VertexBuffer const &vertexbuffer)
	 {
		 _id = vertexbuffer._id;
		 _size = vertexbuffer._size;
		 return (*this);
	 }

	 void VertexBuffer::pushBuffer(Byte *data, size_t size)
	 {
		 _size += size;
		 _buffers.push_back(Buffer(size, data));
	 }

	 size_t VertexBuffer::getSize() const
	 {
		 return (_size);
	 }

	 VertexBuffer::Buffer const &VertexBuffer::getBuffer(size_t index) const
	 {
		 return (_buffers[index]);
	 }

	 void VertexBuffer::setBuffer(VertexBuffer::Buffer const &buffer, size_t index)
	 {
		 _buffers[index] = buffer;
	 }

	 void VertexBuffer::popBuffer()
	 {
		 _size -= _buffers.back().size;
		 _buffers.pop_back();
	 }

	 void VertexBuffer::clearBuffer()
	 {
		 _size = 0;
		 _buffers.clear();
	 }
	
	 void VertexBuffer::transferGPU(bool isIndice, GLenum mode) const
	 {
		GLenum type = isIndice ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
		size_t offset = 0;

		glBindBuffer(type, _id);
		glBufferData(type, _size, NULL, mode);
		handleError();
		for (size_t index = 0; index < _buffers.size(); ++index)
		{
			glBufferSubData(type, offset, _buffers[index].size, _buffers[index].data);
			handleError();
			offset += _buffers[index].size;
		}
	}

	 void VertexBuffer::handleError() const
	 {
		 GLenum error = glGetError();

		 if (error != GL_NO_ERROR)
		 {
			 std::cerr << "Error: check into an VertexBuffer:"<< std::endl;
			 switch (error)
			 {
			 }
		 }
	 }

}