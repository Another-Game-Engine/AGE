#pragma  once

# include <vector>
# include <stdint.h>
# include <memory>
# include <Render/Buffer/IBuffer.hh>

class Buffer;

class BlockMemory
{
public:
	template <typename type_t> BlockMemory(std::shared_ptr<Buffer> const &parent, size_t offset, std::vector<type_t> const &data);
	BlockMemory(BlockMemory const &copy);
	BlockMemory(BlockMemory &&move);
	template <typename type_t> BlockMemory &operator=(std::vector<type_t> const &data);

public:
	BlockMemory &update_buffer(IBuffer const &buffer);
	size_t size() const;

private:
	size_t _offset;
	std::vector<uint8_t> _data;
	std::weak_ptr<Buffer> _parent;
};

template <typename type_t>
BlockMemory & BlockMemory::operator=(std::vector<type_t> const &data)
{
	if (_data.size() < (sizeof(type_t) * data.size())) {
		return (*this);
	}
	std::memcpy(_data.data(), data.data(), _data.size());
	_parent->lock()->require_transfer();
	return (*this);
}

template <typename type_t>
BlockMemory::BlockMemory(std::shared_ptr<Buffer> const &parent, size_t offset, std::vector<type_t> const &data) :
_offset(offset),
_data(data.size() * sizeof(type_t)),
_parent(parent)
{
	std::memcpy(_data.data(), data.data(), _data.size());
	_parent->lock()->require_transfer();
	_parent->lock()->require_resize();
}
