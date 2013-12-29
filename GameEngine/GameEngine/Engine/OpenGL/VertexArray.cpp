#include "VertexArray.hh"
# include "ErrorController.hh"

namespace OpenGLTools
{
	VertexArray::VertexArray()
		: _withIndex(false),
		_id(0),
		_isDrawable(false)
	{
	}

	VertexArray::~VertexArray()
	{
	}

	void VertexArray::init()
	{
		_isDrawable = true;
		glGenVertexArrays(1, &_id);
		_data.init();
		_indices.init();
	}

	void VertexArray::unload()
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
		_attributes.push_back(Attribute(nbrElement, nbrComponent, nbrByte));
		_data.pushBuffer(data, nbrElement * nbrComponent * nbrByte);
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
		_indices.pushBuffer(reinterpret_cast<Byte *>(buffer), nbrElement * sizeof(unsigned int));
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
		if (_isDrawable)
		{
			glBindVertexArray(_id);
			if (_withIndex)
				glDrawElements(draw, _indices.getSize(), GL_UNSIGNED_INT, static_cast<GLvoid const *>(0));
			else
				glDrawArrays(draw, 0, _data.getSize());
			handleError("draw");
			glBindVertexArray(0);
		}
	}

	void VertexArray::transferGPU(GLenum mode) const
	{
		glBindVertexArray(_id);
		_data.transferGPU(false, mode);
		handleError("transferGPU-vertexbuffer");
		if (_withIndex)
		{
			_indices.transferGPU(true, mode);
			handleError("transferGPU-vertexbuffer");
		}
		size_t offset = 0;
		for (size_t index = 0; index < _attributes.size(); ++index)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, (GLint)_attributes[index].nbrComponent, GL_FLOAT, GL_TRUE, 0, (const GLvoid *)offset);
			ErrorController<Attribute>();
			offset += _attributes[index].nbrByte * _attributes[index].nbrComponent * _attributes[index].nbrElement;
		}
		glBindVertexArray(0);
	}

	void VertexArray::handleError(std::string const &localisation) const
	{
		GLenum error;

		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cerr << "Error: check into an VertexArray : -> " << localisation << std::endl;
			switch (error)
			{
			case GL_INVALID_ENUM:
				std::cerr << "Invalid Enum" << std::endl;
				break;
			case GL_INVALID_VALUE:
				std::cerr << "Invalid Value" << std::endl;
				break;
			case GL_INVALID_OPERATION:
				std::cerr << "Invalid Operation" << std::endl;
				break;
			}
		}
	}

}