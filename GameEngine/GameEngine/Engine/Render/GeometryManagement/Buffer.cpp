#include <Render/GeometryManagement/Buffer.hh>

Buffer::Buffer(std::unique_ptr<IBuffer> &&buffer) :
_request_resize(false),
_request_transfer(false),
_size_alloc(0),
_buffer(std::move(buffer))
{

}

Buffer::Buffer(Buffer &&move) :
_request_resize(move._request_resize),
_request_transfer(move._request_transfer),
_size_alloc(move._size_alloc),
_block_memories(std::move(move._block_memories)),
_buffer(std::move(move._buffer))
{

}

size_t Buffer::size() const
{
	return (_buffer->size());
}

Buffer & Buffer::pop_back()
{
	auto less = _block_memories.back().size();
	_size_alloc -= less;
	_block_memories.pop_back();
	require_resize();
	return (*this);
}

BlockMemory & Buffer::operator[](size_t index)
{
	return (_block_memories[index]);
}

Buffer & Buffer::bind()
{
	_buffer->bind();
	return (*this);
}

Buffer & Buffer::unbind()
{
	_buffer->unbind();
	return (*this);
}

Buffer & Buffer::update()
{
	if (_request_resize) {
		_buffer->bind();
		_buffer->alloc(_size_alloc);
		_request_resize = false;
	}
	if (_request_transfer) {
		_buffer->bind();
		for (auto &memory : _block_memories) {
			memory.update_buffer(*_buffer.get());
		}
		_request_transfer = false;
	}
	return (*this);
}

Buffer & Buffer::require_resize()
{
	_request_resize = true;
	return (*this);
}

Buffer & Buffer::require_transfer()
{
	_request_transfer = true;
	return (*this);
}
