#include <Render/ProgramResources/Types/BlockResources.hh>
#include <Render/ProgramResources/Types/UniformBlock.hh>
#include <Render/ProgramResources/Types/ProgramResourcesType.hh>
#include <iostream>

BlockResources::BlockResources(GLint id, std::string &&name, GLenum type, std::tuple<std::weak_ptr<UniformBlock> const &, size_t, size_t, size_t> const &info) :
AProgramResources(id, std::move(name), type),
_parent(std::get<UBO>(info)),
_offset(std::get<OFFSET>(info)),
_size_array(std::get<SIZE_ARRAY>(info)),
_stride(std::get<STRIDE>(info)),
_size((_size_array == 0) ? types[_type].first : _size_array * _stride),
_data(_size, 0)
{
}

BlockResources::BlockResources(BlockResources &&move) :
AProgramResources(std::move(move)),
_parent(std::move(move._parent)),
_offset(move._offset),
_size_array(move._size_array),
_stride(move._stride),
_size(move._size),
_data(std::move(_data))
{

}

size_t BlockResources::offset() const
{
	return (_offset);
}

size_t BlockResources::size_array() const
{
	return (_size_array);
}

size_t BlockResources::stride() const
{
	return (_stride);
}

IProgramResources & BlockResources::operator()()
{
	if (!_update) {
		
		_update = true;
	}
	return (*this);
}

bool BlockResources::safe(size_t s) const
{
	return ((_size == s) ? true : false);
}

size_t BlockResources::size() const
{
	return (_size);
}

void BlockResources::print() const
{
	std::cout << "block resources " << types[_type].second;
	std::cout << " " << _name << ";" <<  std::endl;
}

std::vector<uint8_t> const & BlockResources::data() const
{
	return (_data);
}






