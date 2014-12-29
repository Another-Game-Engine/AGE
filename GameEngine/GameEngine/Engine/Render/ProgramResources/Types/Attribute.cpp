#include <Render/ProgramResources/Types/Attribute.hh>
#include <iostream>

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

IProgramResources & Attribute::operator()()
{
	if (!_update) {
		_buffer.bind();
		glVertexAttribPointer(_id, _available_type.nbr_component, _available_type.type_component, GL_FALSE, 0, 0);
		_update = true;
	}
	return (*this);
}

bool Attribute::safe(size_t s) const
{
	return (s == size());
}

size_t Attribute::size() const
{
	return (_available_type.size);
}

void Attribute::print() const
{
	std::cout << "attribute (location = " << _id << ") " << _available_type.name << " " << _name << ";";
	std::cout << std::endl;
}
