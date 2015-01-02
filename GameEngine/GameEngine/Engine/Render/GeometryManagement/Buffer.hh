#pragma once

# include <Render/GeometryManagement/BlockMemory.hh>
# include <Render/Buffer/IBuffer.hh>
# include <Render/ProgramResources/Types/ProgramResourcesType.hh>

class Buffer
{
public:
	Buffer(std::unique_ptr<IBuffer> &&buffer);
	Buffer(Buffer const &copy) = delete;
	Buffer(Buffer &&move);

public:
	size_t size() const;
	template <typename scalar_t> Buffer &push_back(std::vector<scalar_t> const &data);
	Buffer &pop_back();
	Buffer &bind();
	Buffer &unbind();
	Buffer &update();
	Buffer &require_resize();
	Buffer &require_transfer();
	BlockMemory &operator[](size_t index);

private:
	bool _request_resize;
	bool _request_transfer;
	size_t _size_alloc;
	std::vector<BlockMemory> _block_memories;
	std::unique_ptr<IBuffer> _buffer;
};

template <typename scalar_t>
Buffer & Buffer::push_back(std::vector<scalar_t> const &data)
{
	auto offset = _size_alloc;
	_block_memories.emplace_back(BlockMemory(*this, offset, data));
	_size_alloc += _block_memories.back().size();
	return (*this);
}