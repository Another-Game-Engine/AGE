#pragma once

# include <Render/GeometryManagement/BlockMemory.hh>
# include <Render/Buffer/IBuffer.hh>
# include <Render/ProgramResources/Types/ProgramResourcesType.hh>

class DrawableBuffer
{
public:
	DrawableBuffer(std::unique_ptr<IBuffer> &&buffer);
	DrawableBuffer(DrawableBuffer const &copy);
	DrawableBuffer(DrawableBuffer &&move);

public:
	size_t size() const;
	template <typename scalar_t> DrawableBuffer &push_back(std::vector<scalar_t> const &data);
	DrawableBuffer &pop_back();
	DrawableBuffer &bind();
	DrawableBuffer &unbind();
	DrawableBuffer &update();
	BlockMemory &operator[](size_t index);

private:
	bool _update_memory;
	bool _update_alloc;
	size_t _size_alloc;
	std::vector<BlockMemory> _block_memories;
	std::unique_ptr<IBuffer> _buffer;
};

template <typename scalar_t>
DrawableBuffer & DrawableBuffer::push_back(std::vector<scalar_t> const &data)
{
	auto offset = _size_alloc;
	_block_memories.emplace_back(BlockMemory(*this, offset, data));
	_size_alloc += _block_memories.back().size();
	_update_alloc = false;
	_update_memory = false;
	return (*this);
}