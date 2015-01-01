#include <Render/GeometryManagement/DrawableBuffer.hh>

DrawableBuffer::DrawableBuffer(std::unique_ptr<IBuffer> &&buffer) :
_update_memory(true),
_update_alloc(true),
_size_alloc(0),
_buffer(std::move(buffer))
{

}

DrawableBuffer::DrawableBuffer(DrawableBuffer &&move) :
_update_memory(move._update_memory),
_update_alloc(move._update_alloc),
_size_alloc(move._size_alloc)
{

}

DrawableBuffer::DrawableBuffer(DrawableBuffer const &copy) :
_update_memory(copy._update_memory),
_update_alloc(copy._update_alloc),
_size_alloc(copy._size_alloc)
{

}

size_t DrawableBuffer::size() const
{
	return (_size_alloc);
}

DrawableBuffer & DrawableBuffer::pop_back()
{
	auto less = _block_memories.back().size();
	_size_alloc -= less;
	_block_memories.pop_back();
	_update_alloc = false;
	_update_memory = false;
	return (*this);
}

BlockMemory & DrawableBuffer::operator[](size_t index)
{
	return (_block_memories[index]);
}

DrawableBuffer & DrawableBuffer::bind()
{
	_buffer->bind();
	return (*this);
}

DrawableBuffer & DrawableBuffer::unbind()
{
	_buffer->unbind();
	return (*this);
}

DrawableBuffer & DrawableBuffer::update()
{
	if (!_update_alloc) {
		_buffer->bind();
		_buffer->alloc(_size_alloc);
		_update_alloc = true;
	}
	if (!_update_memory) {
		_buffer->bind();
		for (auto &memory : _block_memories) {
			memory();
		}
	}
	return (*this);
}
