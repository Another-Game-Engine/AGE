#include <Render/GeometryManagement/BlockMemory.hh>
#include <Render/GeometryManagement/Buffer.hh>

BlockMemory::BlockMemory(BlockMemory const &copy) :
_offset(copy._offset),
_data(copy._data),
_parent(copy._parent)
{
}

BlockMemory::BlockMemory(BlockMemory &&move) :
_offset(move._offset),
_data(std::move(move._data)),
_parent(std::move(move._parent))
{
}

BlockMemory & BlockMemory::update_buffer(IBuffer const &buffer)
{
	buffer.sub(_offset, _data.size(), _data.data());
	return (*this);
}

size_t BlockMemory::size() const
{
	return (_data.size());
}