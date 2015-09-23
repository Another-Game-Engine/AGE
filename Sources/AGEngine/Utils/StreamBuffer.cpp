#include <cassert>

#include "StreamBuffer.hpp"

namespace AGE
{
	struct StreamBuffer::BufferData final
	{
		std::size_t size = 0;
		std::size_t capacity = 0;
		unsigned char *data = nullptr;
		unsigned char *ptr = nullptr;
	};

	StreamBuffer::StreamBuffer(void)
		: data(new BufferData)
	{
		return;
	}

	StreamBuffer::StreamBuffer(const StreamBuffer &other)
		: data(other.data)
	{
		return;
	}

	StreamBuffer::StreamBuffer(StreamBuffer &&other)
		: data(std::move(other.data))
	{
		return;
	}

	StreamBuffer &StreamBuffer::operator=(const StreamBuffer &other)
	{
		if (this != &other)
		{
			data = other.data;
		}
		return *this;
	}

	StreamBuffer &StreamBuffer::operator=(StreamBuffer &&other)
	{
		if (this != &other)
		{
			data = std::move(other.data);
		}
		return *this;
	}

	void StreamBuffer::swap(StreamBuffer &other)
	{
		if (this != &other)
		{
			std::swap(data, other.data);
		}
	}

	void StreamBuffer::resize(std::size_t size) const
	{
		allocate(size);
		data->size = size;
	}

	void StreamBuffer::allocate(std::size_t size) const
	{
		if (size <= data->capacity)
		{
			return;
		}
		data->capacity = size;
		unsigned char *newData = new unsigned char[data->capacity];
		std::memcpy(newData, data->data, sizeof(unsigned char) * data->size);
		data->ptr = newData + (data->ptr - data->data);
		delete[] data->data;
		data->data = newData;
	}

	void StreamBuffer::reserve(std::size_t size) const
	{
		if (size <= data->capacity)
		{
			return;
		}
		data->capacity = size * 2;
		unsigned char *newData = new unsigned char[data->capacity];
		std::memcpy(newData, data->data, sizeof(unsigned char) * data->size);
		data->ptr = newData + (data->ptr - data->data);
		delete[] data->data;
		data->data = newData;
	}

	void StreamBuffer::clear(void) const
	{
		data->size = 0;
	}

	std::size_t StreamBuffer::size(void) const
	{
		return data->size;
	}

	unsigned char *StreamBuffer::getData(void) const
	{
		return data->data;
	}

	bool StreamBuffer::endOfFile(void) const
	{
		return static_cast<unsigned int>(data->ptr - data->data) >= data->size;
	}

	int StreamBuffer::getCharacter(void) const
	{
		if (static_cast<unsigned int>(data->ptr - data->data) >= data->size)
		{
			return EOF;
		}
		else
		{
			return *(data->ptr)++;
		}
	}

	void StreamBuffer::seekSet(int offset) const
	{
		data->ptr = data->data + offset;
		assert(data->ptr >= data->data && data->ptr < data->data + data->size && "Bad Offset");
	}

	void StreamBuffer::seekEnd(int offset) const
	{
		data->ptr = data->data + data->size - offset;
		assert(data->ptr >= data->data && data->ptr < data->data + data->size && "Bad Offset");
	}

	void StreamBuffer::seekCurrent(int offset) const
	{
		data->ptr += offset;
		assert(data->ptr >= data->data && data->ptr < data->data + data->size && "Bad Offset");
	}

	std::size_t StreamBuffer::tell(void) const
	{
		return data->ptr - data->data;
	}

	bool StreamBuffer::flush(void) const
	{
		return true;
	}

	bool StreamBuffer::isOpened(void) const
	{
		return true;
	}

	std::size_t StreamBuffer::read(void *ptr, std::size_t size, std::size_t nmemb) const
	{
		assert(size != 0 && "Invalid size");
		const std::size_t bytes = size * nmemb;
		const std::size_t bytesToEnd = data->size - (data->ptr - data->data);
		if (bytes <= bytesToEnd)
		{
			std::memcpy(ptr, data->ptr, bytes);
			data->ptr += bytes;
			return nmemb;
		}
		else
		{
			std::memcpy(ptr, data->ptr, bytesToEnd);
			data->ptr += bytesToEnd;
			return bytesToEnd / size;
		}
	}

	std::size_t StreamBuffer::write(const void *ptr, std::size_t size, std::size_t nmemb) const
	{
		assert(size != 0 && "Invalid size");
		const std::size_t bytes = size * nmemb;
		const std::size_t bytesToEnd = data->size - (data->ptr - data->data);
		if (bytes <= bytesToEnd)
		{
			std::memcpy(data->ptr, ptr, bytes);
			data->ptr += bytes;
			return nmemb;
		}
		else
		{
			const std::size_t newSize = data->size + static_cast<int>(bytes - bytesToEnd);
			reserve(newSize);
			resize(newSize);
			std::memcpy(data->ptr, ptr, bytes);
			data->ptr += bytes;
			return nmemb;
		}
	}

	StreamBuffer::~StreamBuffer(void)
	{
		delete[] data->data;
	}

	void swap(StreamBuffer &lhs, StreamBuffer &rhs)
	{
		lhs.swap(rhs);
	}
}