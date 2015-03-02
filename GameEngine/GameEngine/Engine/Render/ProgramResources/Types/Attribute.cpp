#include <Render/ProgramResources/Types/Attribute.hh>
#include <Render/GeometryManagement/Buffer/Buffer.hh>
#include <iostream>

namespace AGE
{
	Attribute::Attribute(GLint index, GLuint location, std::string &&name, GlType const &type) :
		AProgramResources(index, std::move(name), GL_PROGRAM_INPUT),
		_location(location),
		_available_type(type)
	{

	}

	Attribute::Attribute(Attribute &&move) :
		AProgramResources(std::move(move)),
		_available_type(std::move(move._available_type))
	{

	}

	Attribute::Attribute(Attribute const &copy) :
		AProgramResources(copy),
		_available_type(copy._available_type)
	{

	}

	IProgramResources & Attribute::update()
	{
		_buffer->bind();
		glEnableVertexAttribArray(_location);
		glVertexAttribPointer(_location, _available_type.nbr_component, _available_type.type_component, GL_FALSE, 0, 0);
		return (*this);
	}

	size_t Attribute::size() const
	{
		return (_available_type.size);
	}

	bool Attribute::safe(size_t s) const
	{
		return (s == sizeof(type_t));
	}

	void Attribute::print() const
	{
		std::cout << "attribute (location = " << _id << ") " << _available_type.name << " " << _name << ";";
		std::cout << std::endl;
	}

	Attribute &Attribute::operator=(std::shared_ptr<Buffer> const &buffer)
	{
		_buffer = buffer;
		return *this;
	}

	bool Attribute::operator==(std::pair<GLenum, std::string> const &p) const
	{
		return (_available_type == p.first && _name == p.second);
	}

	bool Attribute::operator!=(std::pair<GLenum, std::string> const &p) const
	{
		return (!(*this == p));
	}
}