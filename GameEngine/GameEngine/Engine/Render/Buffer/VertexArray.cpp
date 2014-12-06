#include <Render/Buffer/VertexArray.hh>

VertexArray::VertexArray() :
_id(0)
{
	glGenVertexArrays(1, &_id);
}

VertexArray::~VertexArray()
{
	if (_id)
		glDeleteVertexArrays(1, &_id);
}

VertexArray::VertexArray(VertexArray &&move) :
_id(move._id)
{
	_id = 0;
}

/**
* Method:    bind
* FullName:  VertexArray::bind
* Access:    public 
* Returns:   void
* Qualifier: const
* Goal:		 bind the VAO to the context
*/
void VertexArray::bind() const
{
	glBindVertexArray(_id);
}

/**
* Method:    unbind
* FullName:  VertexArray::unbind
* Access:    public 
* Returns:   void
* Qualifier: const
* Goal:		 unbind the VAO to the context
*/
void VertexArray::unbind() const
{
	glBindVertexArray(0);
}

/**
* Method:    getId
* FullName:  VertexArray::getId
* Access:    public 
* Returns:   GLuint
* Qualifier: const
* Goal:		 get the id of the VAO
*/
GLuint VertexArray::getId() const
{
	return (_id);
}

/**
* Method:    attribute
* FullName:  VertexArray::attribute
* Access:    public 
* Returns:   VertexArray const &
* Qualifier: const
* Parameter: size_t index
* Parameter: Attribute type
* Parameter: IBuffer const & buffer
* Goal:		 set the attribute into the BO target
*/
VertexArray const &VertexArray::attribute(size_t index, Attribute type, IBuffer const &buffer) const
{
	buffer.bind();
	glVertexAttribPointer(index, std::get<Property::NbrComponent>(attProperty[type]), std::get<Property::GLType>(attProperty[type]), GL_FALSE, 0, 0);
	return (*this);
}

/**
* Method:    attribute
* FullName:  VertexArray::attribute
* Access:    public 
* Returns:   VertexArray const &
* Qualifier: const
* Parameter: size_t index
* Parameter: Attribute type
* Goal:		 set the attribute to the current context
*/
VertexArray const &VertexArray::attribute(size_t index, Attribute type) const
{
	glVertexAttribPointer(index, std::get<Property::NbrComponent>(attProperty[type]), std::get<Property::GLType>(attProperty[type]), GL_FALSE, 0, 0);
	return (*this);
}