#pragma  once

# include <vector>
# include <stdint.h>
# include <memory>

class Attribute;

class BlockMemory
{
public:
	template <typename type_t> BlockMemory(Attribute const &parent, size_t offset, std::vector<type_t> const &data);
	BlockMemory(BlockMemory const &copy);
	BlockMemory(BlockMemory &&move);
	template <typename type_t> BlockMemory &operator=(std::vector<type_t> const &data);

public:
	BlockMemory &operator()();
	size_t size() const;

private:
	bool _update;
	size_t _offset;
	std::vector<uint8_t> _data;
	Attribute const &_parent;
};

template <typename type_t>
BlockMemory & BlockMemory::operator=(std::vector<type_t> const &data)
{
	if (_data.size() < (sizeof(type_t) * data.size())) {
		return (*this);
	}
	std::memcpy(_data.data(), data.data(), _data.size());
	_update = false;
	return (*this);
}

template <typename type_t>
BlockMemory::BlockMemory(Attribute const &parent, size_t offset, std::vector<type_t> const &data) :
_update(false),
_offset(offset),
_data(data.size() * sizeof(type_t)),
_parent(parent)
{
	std::memcpy(_data.data(), data.data(), _data.size());
}
