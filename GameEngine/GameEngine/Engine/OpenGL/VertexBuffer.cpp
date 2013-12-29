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

<<<<<<< HEAD
/// draw with the elements the contain of vbo
void VertexBuffer::draw(GLenum mode) const
{
  assert(_drawable == true && "Cannot draw, buffer not initialized correctly");
  glBindBuffer(GL_ARRAY_BUFFER, _idBuffer);
  for (size_t i = 0; i < _attribute.size(); ++i)
  {
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, _attribute[i].nbrCompenent, _attribute[i].type, GL_FALSE, 0, (const GLvoid*)(_attribute[i].offset));
  }
  if (_drawable)
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _idIndex);
    glDrawElements(mode, _nbrVertex, GL_UNSIGNED_INT, reinterpret_cast<GLvoid *>(0));
  }
  else
  {
    glDrawArrays(mode, 0, _nbrVertex);
  }
}

/// add a new attribute
void VertexBuffer::addAttribute(Attribute const &attribute)
{
  Attribute r(attribute);
  
  if (_attribute.size() > 0)
    {
      r.offset = _attribute.back().offset + _attribute.back().nbrByte * _nbrVertex;
    }
  glBindBuffer(GL_ARRAY_BUFFER, _idBuffer);
  size_t size = r.offset + r.nbrByte * _nbrVertex;
  glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW);
  _attribute.push_back(r);
}

Attribute *VertexBuffer::getAttribute(unsigned int index)
{
	if (index >= _attribute.size())
		return nullptr;
	return &(_attribute[index]);
}

/// delete a Attribute
void VertexBuffer::clearAttributes(void)
{
  _attribute.clear();
}
=======
	 void VertexBuffer::unload()
	 {
		 glDeleteBuffers(1, &_id);
	 }
	 
	 VertexBuffer::VertexBuffer(VertexBuffer const &copy)
		 : _id(copy._id),
		 _size(copy._size)
	 {
>>>>>>> 2e79899faeb4f4542f69433c05fa9d4f84cb4912

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