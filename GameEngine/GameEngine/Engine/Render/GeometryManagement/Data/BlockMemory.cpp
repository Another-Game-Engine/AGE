#include <Render/GeometryManagement/Data/BlockMemory.hh>
#include <Render/GeometryManagement/Buffer/Buffer.hh>

namespace AGE
{
	BlockMemory::BlockMemory(Buffer &parent, size_t index, size_t offset, std::vector<uint8_t> const &data) :
		_is_up_to_date(false),
		_index(index),
		_offset(offset),
		_data(data.size()),
		_parent(parent)
	{
		std::memcpy(_data.data(), data.data(), _data.size());
		_parent.require_transfer();
		_parent.require_resize();
	}


	BlockMemory::BlockMemory(BlockMemory const &copy) :
		_is_up_to_date(copy._is_up_to_date),
		_index(copy._index),
		_offset(copy._offset),
		_data(copy._data),
		_parent(copy._parent)
	{
	}

	BlockMemory::BlockMemory(BlockMemory &&move) :
		_is_up_to_date(move._is_up_to_date),
		_index(move._index),
		_offset(move._offset),
		_data(std::move(move._data)),
		_parent(std::move(move._parent))
	{
	}

	BlockMemory & BlockMemory::setDatas(std::vector<uint8_t> const &data)
	{
		if (_data.size() < data.size()) {
			return (*this);
		}
		std::memcpy(_data.data(), data.data(), _data.size());
		_parent.require_transfer();
		_is_up_to_date = false;
		return (*this);
	}

	BlockMemory & BlockMemory::update_buffer(ABuffer const &buffer)
	{
		buffer.sub(_offset, _data.size(), _data.data());
		_is_up_to_date = true;
		return (*this);
	}

	size_t BlockMemory::size() const
	{
		return (_data.size());
	}

	size_t BlockMemory::offset() const
	{
		return (_offset);
	}

	BlockMemory & BlockMemory::reset(size_t index, size_t offset)
	{
		_offset = offset;
		_index = index;
		needUpdate();
		return (*this);
	}

	void BlockMemory::needUpdate()
	{
		_is_up_to_date = false;
	}

	BlockMemory & BlockMemory::remove()
	{
		_parent.erase(_index);
		_parent.require_resize();
		_parent.require_transfer();
		return (*this);
	}

	std::vector<uint8_t> const & BlockMemory::get() const
	{
		return (_data);
	}

	bool BlockMemory::is_up_to_date() const
	{
		return (_is_up_to_date);
	}
}