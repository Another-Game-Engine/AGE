#include "VertexArray.hh"

namespace OpenGLTools
{
	VertexArray::VertexArray()
		: _withIndex(false)
	{
		glGenVertexArrays(1, &_id);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &_id);
	}

	VertexArray::VertexArray(VertexArray const &copy)
		: _withIndex(copy._withIndex),
		_attributes(copy._attributes),
		_data(copy._data),
		_indices(copy._indices),
		_id(copy._id)
	{
	}

	VertexArray &VertexArray::operator=(VertexArray const &vertexarray)
	{
		_withIndex = vertexarray._withIndex;
		_attributes = vertexarray._attributes;
		_data = vertexarray._data;
		_indices = vertexarray._indices;
		_id = vertexarray._id;
		return (*this);
	}

	VertexArray::Attribute::Attribute(size_t nbrElement,
		unsigned char nbrComponent,
		unsigned char nbrByte)
		: nbrElement(nbrElement),
		nbrComponent(nbrComponent),
		nbrByte(nbrByte)
	{
	}

	VertexArray::Attribute::~Attribute()
	{
	}

	VertexArray::Attribute::Attribute(Attribute const &attribute)
		: nbrElement(attribute.nbrElement),
		nbrComponent(attribute.nbrComponent),
		nbrByte(attribute.nbrByte)
	{
	}

	VertexArray::Attribute &VertexArray::Attribute::operator=(VertexArray::Attribute const &attribute)
	{
		nbrElement = attribute.nbrElement;
		nbrComponent = attribute.nbrComponent;
		nbrByte = attribute.nbrByte;
		return (*this);
	}


	void VertexArray::addAttribute(size_t nbrElement, unsigned char nbrComponent, unsigned char nbrByte, Byte *data)
	{
		glBindVertexArray(_id);
		_attributes.push_back(Attribute(nbrElement, nbrComponent, nbrByte));
		_data.pushBuffer(data, nbrElement * nbrComponent * nbrByte);
		glBindVertexArray(0);
	}

	VertexBuffer const &VertexArray::getIndices() const
	{
		return (_indices);
	}

	VertexBuffer const &VertexArray::getData() const
	{
		return (_data);
	}

	void VertexArray::setIndices(size_t nbrElement, unsigned int *buffer)
	{
		_withIndex = true;
		_indices.clearBuffer();
		_indices.pushBuffer(reinterpret_cast<Byte *>(buffer), nbrElement);
	}

	void VertexArray::setData(VertexBuffer const &vertexbuffer)
	{
		_data = vertexbuffer;
	}

	void VertexArray::clearIndices()
	{
		_indices.clearBuffer();
	}

	void VertexArray::clearData()
	{
		_data.clearBuffer();
	}

	void VertexArray::draw(GLenum draw) const
	{
		glBindVertexArray(_id);
		if (_withIndex)
			glDrawElements(draw, _indices.getSize(), GL_UNSIGNED_BYTE, static_cast<GLvoid const *>(0));
		else
			glDrawArrays(draw, 0, _data.getSize());
		handleError();
		glBindVertexArray(0);
	}

	void VertexArray::transferGPU(GLenum mode) const
	{
		size_t offset = 0;
		glBindVertexArray(_id);
		for (size_t index = 0; index < _attributes.size(); ++index)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, _attributes[index].nbrComponent, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)offset);
			handleError();
			offset += _attributes[index].nbrByte * _attributes[index].nbrComponent * _attributes[index].nbrElement;
		}
		_data.transferGPU(_withIndex, mode);
		handleError();
		glBindVertexArray(0);
	}

	void VertexArray::handleError() const
	{
		GLenum error;

		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cerr << "Error: check into an VertexArray:" << std::endl;
			switch (error)
			{
			case GL_INVALID_ENUM:
				std::cerr << " - this mode is not accepted." << std::endl;
				break;
			case GL_INVALID_VALUE:
				std::cerr << " - the count set is negative." << std::endl;
				break;
			case GL_INVALID_OPERATION:
				std::cerr << " - A non-zero buffer object name is bound to an enabled array."
						  << std::endl
						  << " - or the element array and the buffer object's data store is currently mapped."
						  << std::endl;
				break;
			}
		}
	}

}