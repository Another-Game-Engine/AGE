#include <Utils/Profiler.hpp>
#include <Render/GeometryManagement/Buffer/Buffer.hh>

namespace AGE
{
	Buffer::Buffer(std::string &&name, std::unique_ptr<IBuffer> &&buffer) :
		_request_resize(false),
		_request_transfer(false),
		_size_alloc(0),
		_buffer(std::move(buffer)),
		_name(std::move(name))
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

	Buffer & Buffer::clear()
	{
		_size_alloc = 0;
		_block_memories.clear();
		require_resize();
		return (*this);
	}

	std::shared_ptr<BlockMemory> const & Buffer::operator[](size_t index)
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
		if (_request_resize)
		{
			_buffer->bind();
			_buffer->alloc(_size_alloc);
			_request_resize = false;
			_request_transfer = true;

		}
		if (_request_transfer)
		{
			_buffer->bind();
			for (auto &memory : _block_memories)
			{
				if (!memory->is_up_to_date())
				{
					memory->update_buffer(*_buffer.get());
				}
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

	std::shared_ptr<BlockMemory> const &Buffer::push_back(std::vector<uint8_t> &&data)
	{
		auto offset = _size_alloc;
		_block_memories.emplace_back(std::make_shared<BlockMemory>(*this, _block_memories.size(), offset, data));
		_size_alloc += _block_memories.back()->size();
		for (auto index = 0; index < _block_memories.size(); ++index)
		{
			_block_memories[index]->needUpdate();
		}
		return (_block_memories.back());
	}

	Buffer & Buffer::erase(size_t index)
	{
		auto &iterator = _block_memories.begin() + index;
		_size_alloc -= iterator->get()->size();
		_block_memories.erase(iterator);
		auto offset = 0ull;
		for (auto index = 0; index < _block_memories.size(); ++index)
		{
			_block_memories[index]->reset(index, offset);
			offset += _block_memories[index]->size();
		}
		return (*this);
	}
}
