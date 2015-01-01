#include <Render/ProgramResources/Types/Attribute.hh>
#include <iostream>

Attribute::Attribute(GLint index, std::string &&name, GlType const &type) :
AProgramResources(index, std::move(name), GL_PROGRAM_INPUT),
_update_memory(true),
_update_alloc(true),
_size_alloc(0),
_available_type(type)
{
	
}

Attribute::Attribute(Attribute &&move) :
AProgramResources(std::move(move)),
_update_memory(move._update_memory),
_update_alloc(move._update_alloc),
_size_alloc(move._size_alloc),
_available_type(std::move(move._available_type))
{

}

Attribute::Attribute(Attribute const &copy) :
AProgramResources(copy),
_update_memory(copy._update_memory),
_update_alloc(copy._update_alloc),
_size_alloc(copy._size_alloc),
_available_type(copy._available_type)
{

}

IProgramResources & Attribute::operator()()
{
	if (!_update_alloc) {
		_buffer.bind();
		_buffer.alloc(_size_alloc);
		_update_alloc = true;
	}
	if (!_update_memory) {
		_buffer.bind();
		for (auto &memory : _block_memories) {
			memory();
		}
	}
	return (*this);
}

bool Attribute::safe(size_t s) const
{
	return (s == sizeof(type_t));
}

size_t Attribute::size() const
{
	return (_block_memories.size());
}

void Attribute::print() const
{
	std::cout << "attribute (location = " << _id << ") " << _available_type.name << " " << _name << ";";
	std::cout << std::endl;
}

VertexBuffer const & Attribute::buffer() const
{
	return (_buffer);
}

Attribute & Attribute::update()
{
	return (*this);
}

Attribute & Attribute::pop_back()
{
	auto less = _block_memories.back().size();
	_size_alloc -= less;
	_block_memories.pop_back();
	_update_alloc = false;
	_update_memory = false;
	return (*this);
}

BlockMemory & Attribute::operator[](size_t index)
{
	return (_block_memories[index]);
}

Attribute & Attribute::bind()
{
	_buffer.bind();
	glVertexAttribPointer(_id, _available_type.nbr_component, _available_type.type_component, GL_FALSE, 0, 0);
	return (*this);
}

Attribute & Attribute::unbind()
{
	_buffer.unbind();
	return (*this);
}
