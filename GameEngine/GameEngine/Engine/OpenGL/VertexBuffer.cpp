#include "VertexBuffer.hh"

#include <assert.h>

namespace OpenGLTools
{
	VertexBuffer::VertexBuffer(bool isIndices)
		: _id(0),
		_isBind(false)
	{
		_mode = isIndices ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
	}

	 VertexBuffer::~VertexBuffer()
	 {
		 if (_id)
		   glDeleteBuffers(1, &_id);
	 }

	 void VertexBuffer::init()
	 {
		 glGenBuffers(1, &_id);
	 }

	 void VertexBuffer::unload()
	 {
		 if (_id)
		 {
			 glDeleteBuffers(1, &_id);
			 _id = 0;
		 }
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
		 if (_isBind == false)
		 {
			 _isBind = true;
			 glBindBuffer(_mode, _id);
		 }
	 }

	 void VertexBuffer::unbind()
	 {
		 _isBind = false;
		 glBindBuffer(_mode, 0);
	 }
}