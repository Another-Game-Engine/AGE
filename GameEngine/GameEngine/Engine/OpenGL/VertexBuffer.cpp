#include "VertexBuffer.hh"

#include <assert.h>

namespace OpenGLTools
{

	VertexBuffer::VertexBuffer(void)
		: _idBuffer(0),
		_idIndex(0),
		_drawable(false)
	{
	}

	VertexBuffer::~VertexBuffer(void)
	{
		if (_idBuffer != 0)
			glDeleteBuffers(1, &_idBuffer);
		if (_idIndex != 0)
			glDeleteBuffers(1, &_idIndex);
	}

	/// init the vertex buffer with information static
	void VertexBuffer::init(size_t nbrVertex, unsigned int *vertex)
	{
		if (_idBuffer != 0)
			glDeleteBuffers(1, &_idBuffer);
		if (_idIndex != 0)
			glDeleteBuffers(1, &_idIndex);
		_nbrVertex = nbrVertex;
		if (vertex)
		{
			glGenBuffers(2, &_idBuffer);
			_drawable = true;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _idIndex);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* nbrVertex, vertex, GL_DYNAMIC_DRAW);
		}
		else
			glGenBuffers(1, &_idBuffer);
	}

	/// Fill one blocks
	void VertexBuffer::setBuffer(size_t index, byte *buffer) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, _idBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, _attribute[index].offset, _attribute[index].nbrByte * _nbrVertex, buffer);
	}

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

	/// delete a Attribute
	void VertexBuffer::clearAttributes(void)
	{
		_attribute.clear();
	}

	GLuint VertexBuffer::getId() const
	{
		return (_idBuffer);
	}

}