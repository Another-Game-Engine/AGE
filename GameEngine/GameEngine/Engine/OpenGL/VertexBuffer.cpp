#include "VertexBuffer.hh"

#include <assert.h>

namespace OpenGLTools
{
	VertexBuffer::VertexBuffer()
		: _id(0)
	{
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
		 : _id(copy._id)
	 {

	 }

	 VertexBuffer &VertexBuffer::operator=(VertexBuffer const &vertexbuffer)
	 {
		 _id = vertexbuffer._id;
		 return (*this);
	 }

}