#include "VertexBuffer.hh"

namespace gl
{
	VertexBuffer::VertexBuffer(bool isIndices)
		: _id(0),
		_isBind(false)
	{
		_mode = isIndices ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
		glGenBuffers(1, &_id);
	}

	 VertexBuffer::~VertexBuffer()
	 {
		 if (_id)
		   glDeleteBuffers(1, &_id);
	 }
	 
	 VertexBuffer::VertexBuffer(VertexBuffer const &copy)
		 : _id(copy._id),
		 _mode(copy._mode),
		 _isBind(copy._isBind)
	 {

	 }

	 VertexBuffer &VertexBuffer::operator=(VertexBuffer const &vertexbuffer)
	 {
		 _id = vertexbuffer._id;
		 _mode = vertexbuffer._mode;
		 _isBind = vertexbuffer._isBind;
		 return (*this);
	 }

	 void VertexBuffer::bind()
	 {
		glBindBuffer(_mode, _id);
	 }

	 void VertexBuffer::unbind()
	 {
		glBindBuffer(_mode, 0);
	 }

	 GLenum VertexBuffer::getMode() const
	 {
		 return (_mode);
	 }

	 GLuint VertexBuffer::getId() const
	 {
		 return (_id);
	 }
}