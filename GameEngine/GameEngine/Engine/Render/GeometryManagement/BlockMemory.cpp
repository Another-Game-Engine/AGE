#include <Render/GeometryManagement/BlockMemory.hh>
#include <Render/GeometryManagement/Buffer.hh>

BlockMemory::BlockMemory(Buffer &parent, size_t offset, std::vector<uint8_t> const &data) :
_offset(offset),
_data(data.size()),
_parent(parent)
{
	std::memcpy(_data.data(), data.data(), _data.size());
	_parent.require_transfer();
	_parent.require_resize();
}


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

BlockMemory & BlockMemory::operator=(std::vector<uint8_t> const &data)
{
	if (_data.size() < data.size()) {
		return (*this);
	}
	std::memcpy(_data.data(), data.data(), _data.size());
	_parent.require_transfer();
	return (*this);
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
