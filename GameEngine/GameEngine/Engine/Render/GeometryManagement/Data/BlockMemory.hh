#pragma  once

# include <vector>
# include <stdint.h>
# include <memory>
# include <Render/Buffer/IBuffer.hh>

namespace AGE
{
	class Buffer;

	class BlockMemory
	{
	public:
		BlockMemory(Buffer &parent, size_t index, size_t offset, std::vector<uint8_t> const &data);
		BlockMemory(BlockMemory const &copy);
		BlockMemory(BlockMemory &&move);
		BlockMemory &operator=(std::vector<uint8_t> const &data);

	public:
		bool is_update() const;
		BlockMemory &update_buffer(IBuffer const &buffer);
		BlockMemory &reset(size_t index, size_t offset);
		BlockMemory &remove();
		std::vector<uint8_t> const &get() const;
		size_t size() const;
		size_t offset() const;

	private:
		bool _is_update;
		size_t _index;
		size_t _offset;
		std::vector<uint8_t> _data;
		Buffer &_parent;
	};
}