#include <Render/ProgramResources/Types/Attribute.hh>
#include <iostream>

namespace AGE
{
	Attribute::Attribute(GLint index, std::string &&name, GlType const &type) :
		AProgramResources(index, std::move(name), GL_PROGRAM_INPUT),
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

	bool Attribute::operator==(GLenum p) const
	{
		return (_available_type == p);
	}

	bool Attribute::operator!=(GLenum p) const
	{
		return (!(_available_type == p));
	}
}