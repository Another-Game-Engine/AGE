#include <cassert>

#include "Buffer.hpp"

namespace Age
{
	namespace Engine
	{
		struct Buffer::BufferData final
		{
			std::size_t size = 0;
			std::size_t capacity = 0;
			unsigned char *data = nullptr;
			unsigned char *ptr = nullptr;
		};

		Buffer::Buffer(void)
			: data(new BufferData)
		{
			return;
		}

		Buffer::Buffer(const Buffer &other)
			: data(other.data)
		{
			return;
		}

		Buffer::Buffer(Buffer &&other)
			: data(std::move(other.data))
		{
			return;
		}

		Buffer &Buffer::operator=(const Buffer &other)
		{
			if (this != &other)
			{
				data = other.data;
			}
			return *this;
		}

		Buffer &Buffer::operator=(Buffer &&other)
		{
			if (this != &other)
			{
				data = std::move(other.data);
			}
			return *this;
		}

		void Buffer::swap(Buffer &other)
		{
			if (this != &other)
			{
				std::swap(data, other.data);
			}
		}

		void Buffer::resize(std::size_t size) const
		{
			allocate(size);
			data->size = size;
		}

		void Buffer::allocate(std::size_t size) const
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

		void Buffer::reserve(std::size_t size) const
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

		void Buffer::clear(void) const
		{
			data->size = 0;
		}

		std::size_t Buffer::size(void) const
		{
			return data->size;
		}

		unsigned char *Buffer::getData(void) const
		{
			return data->data;
		}

		bool Buffer::endOfFile(void) const
		{
			return static_cast<unsigned int>(data->ptr - data->data) >= data->size;
		}

		int Buffer::getCharacter(void) const
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

		void Buffer::seekSet(int offset) const
		{
			data->ptr = data->data + offset;
			assert(data->ptr >= data->data && data->ptr < data->data + data->size && "Bad Offset");
		}

		void Buffer::seekEnd(int offset) const
		{
			data->ptr = data->data + data->size - offset;
			assert(data->ptr >= data->data && data->ptr < data->data + data->size && "Bad Offset");
		}

		void Buffer::seekCurrent(int offset) const
		{
			data->ptr += offset;
			assert(data->ptr >= data->data && data->ptr < data->data + data->size && "Bad Offset");
		}

		std::size_t Buffer::tell(void) const
		{
			return data->ptr - data->data;
		}

		bool Buffer::flush(void) const
		{
			return true;
		}

		bool Buffer::isOpened(void) const
		{
			return true;
		}

		std::size_t Buffer::read(void *ptr, std::size_t size, std::size_t nmemb) const
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

		std::size_t Buffer::write(const void *ptr, std::size_t size, std::size_t nmemb) const
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

		Buffer::~Buffer(void)
		{
			delete[] data->data;
		}

		void swap(Buffer &lhs, Buffer &rhs)
		{
			lhs.swap(rhs);
		}
	}
}