#include <Render/ProgramResources/Types/Attribute.hh>
#include <Render/GeometryManagement/Buffer/Buffer.hh>
#include <iostream>
#include <Utils/StringID.hpp>

namespace AGE
{
	Attribute::Attribute(GLint index, GLuint location, const StringID &name, GlType const &type) :
		AProgramResources(index, name, GL_PROGRAM_INPUT),
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
		if (_buffer)
		{
			_buffer->bind();
			glEnableVertexAttribArray(_location);
			glVertexAttribPointer(_location, _available_type.nbr_component, _available_type.type_component, GL_FALSE, 0, 0);
		}
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
		std::cout << "attribute (location = " << _id << ") " << _available_type.name << " " << _name.str() << ";";
		std::cout << std::endl;
	}

	Attribute &Attribute::operator=(std::shared_ptr<Buffer> const &buffer)
	{
		_buffer = buffer;
		return *this;
	}

	bool Attribute::operator==(std::pair<GLenum, StringID> const &p) const
	{
		return (_available_type == p.first && _name == p.second);
	}

	bool Attribute::operator!=(std::pair<GLenum, StringID> const &p) const
	{
		return (!(*this == p));
	}
}