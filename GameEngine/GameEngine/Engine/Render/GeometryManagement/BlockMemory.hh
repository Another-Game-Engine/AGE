#pragma  once

# include <vector>
# include <stdint.h>
# include <memory>
# include <Render/Buffer/IBuffer.hh>

class Buffer;

class BlockMemory
{
public:
	BlockMemory(Buffer &parent, size_t offset, std::vector<uint8_t> const &data);
	BlockMemory(BlockMemory const &copy);
	BlockMemory(BlockMemory &&move);
	BlockMemory &operator=(std::vector<uint8_t> const &data);

public:
	BlockMemory &update_buffer(IBuffer const &buffer);
	size_t size() const;

private:
	size_t _offset;
	std::vector<uint8_t> _data;
	Buffer &_parent;
};
