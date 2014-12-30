#include <Render/GeometryManagement/BlockMemory.hh>
#include <Render/ProgramResources/Types/Attribute.hh>

BlockMemory::BlockMemory(BlockMemory const &copy) :
_update(copy._update),
_offset(copy._offset),
_data(copy._data),
_parent(copy._parent)
{
}

BlockMemory::BlockMemory(BlockMemory &&move) :
_update(move._update),
_offset(move._offset),
_data(std::move(move._data)),
_parent(std::move(move._parent))
{
}

BlockMemory & BlockMemory::operator()()
{
	if (!_update) {
		_parent.buffer().sub(_offset, _data.size(), _data.data());
		_update = true;
	}
	return (*this);
}

size_t BlockMemory::size() const
{
	return (_data.size());
}